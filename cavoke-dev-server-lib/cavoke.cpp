#include <drogon/drogon.h>
#include <boost/algorithm/string/join.hpp>
#include <boost/program_options.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include "controllers/logic_controller.h"

namespace cavoke::logic_server {

void run(const std::string &ip, uint16_t port) {
    auto &app = drogon::app();

    app.addListener(ip, port);

    // print db and listeners on start
    app.registerBeginningAdvice([]() {
        auto listeners = drogon::app().getListeners();
        std::string ips =
            listeners.empty()
                ? "<NO LISTENERS>"
                : boost::join(
                      listeners | boost::adaptors::transformed(
                                      [](const trantor::InetAddress &e) {
                                          return e.toIpPort();
                                      }),
                      ", ");
        std::cout << "Listening at " << ips << "... " << std::endl;
    });

    app.registerPostHandlingAdvice([](const drogon::HttpRequestPtr &req,
                                      const drogon::HttpResponsePtr &resp) {
        // cors header for every response
        resp->addHeader("Access-Control-Allow-Origin", "*");
    });

    auto logic_controller = std::make_shared<controllers::LogicController>();

    app.registerController(logic_controller);

    app.run();
}

}  // namespace cavoke::logic_server

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
    po::options_description desc("Allowed options");
    auto add_desc_options = desc.add_options();
    add_desc_options("help,h", "Print help");
    add_desc_options("ip,i", po::value<std::string>()->default_value("0.0.0.0"),
                     "IP address for connection");
    add_desc_options("port,p", po::value<uint16_t>()->default_value(9090),
                     "TCP/IP port number for connection");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }

    // defaults
    std::string ip = "0.0.0.0";
    uint16_t port = 9090;

    if (vm.count("ip")) {
        ip = vm["ip"].as<std::string>();
    }

    if (vm.count("port")) {
        port = vm["port"].as<uint16_t>();
    }

    cavoke::logic_server::run(ip, port);

    return 0;
}
