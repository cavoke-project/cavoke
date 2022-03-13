#include <drogon/HttpAppFramework.h>
#include <boost/program_options.hpp>
#include "controllers/games_controller.h"
#include "controllers/sessions_controller.h"
#include "controllers/state_controller.h"
#include "model/game_logic_manager.h"
#include "model/games_storage.h"
#include "model/sessions/sessions_storage.h"

namespace cavoke::server {
void run(const std::string &host,
         uint16_t port,
         const std::string &config_file) {
    // TODO: logging

    // init models
    std::cout << "Initialize models..." << std::endl;
    auto games_storage = std::make_shared<model::GamesStorage>(
        model::GamesStorageConfig::load(config_file));
    auto game_logic_manager =
        std::make_shared<model::GameLogicManager>(games_storage);
    auto game_state_storage = std::make_shared<model::GameStateStorage>();
    auto participation_storage = std::make_shared<model::SessionsStorage>();
    auto authentication_manager =
        std::make_shared<model::AuthenticationManager>();

    // init controllers
    std::cout << "Initialize controllers..." << std::endl;
    auto games_controller =
        std::make_shared<controllers::GamesController>(games_storage);
    auto state_controller = std::make_shared<controllers::StateController>(
        games_storage, game_logic_manager, game_state_storage,
        participation_storage);
    auto sessions_controller =
        std::make_shared<controllers::SessionsController>(
            games_storage, game_logic_manager, game_state_storage,
            participation_storage, authentication_manager);

    auto &app = drogon::app();

    // register controllers
    app.registerController(games_controller);
    app.registerController(state_controller);
    app.registerController(sessions_controller);

    // start server
    std::cout << "Listening at " << host << ":" << port << std::endl;
    app.addListener(host, port).run();
}
}  // namespace cavoke::server

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "Print help")(
        "config-file,c", po::value<std::string>(),
        "File with game storage configuration")(
        "host,ip,a", po::value<std::string>()->default_value("0.0.0.0"),
        "Host on which server is located")(
        "port,p", po::value<uint16_t>()->default_value(8080),
        "TCP/IP port number for connection");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }

    std::string config_file;
    if (vm.count("config-file")) {
        config_file = vm["config-file"].as<std::string>();
    } else {
        //    config_file = std::getenv("CAVOKE_SERVER_CONFIG"); // TODO: think
        //    and discuss
    }

    std::string host = vm.at("host").as<std::string>();
    uint16_t port = vm.at("port").as<uint16_t>();

    cavoke::server::run(host, port, config_file);
    return 0;
}
