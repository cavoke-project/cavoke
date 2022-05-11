#include <drogon/HttpAppFramework.h>
#include <boost/algorithm/string/join.hpp>
#include <boost/program_options.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include "controllers/games_controller.h"
#include "controllers/sessions_controller.h"
#include "controllers/state_controller.h"
#include "controllers/statistics_controller.h"
#include "model/games/games_storage.h"
#include "model/logic/game_logic_manager.h"
#include "model/sessions/sessions_storage.h"
#include "model/statistics/statistics_manager.h"

namespace cavoke::server {
void run(const model::GamesStorageConfig &games_storage_config) {
    // TODO: logging

    // init models
    std::cout << "Initialize models..." << std::endl;
    auto games_storage =
        std::make_shared<model::GamesStorage>(games_storage_config);
    auto game_logic_manager =
        std::make_shared<model::GameLogicManager>(games_storage);
    auto game_state_storage = std::make_shared<model::GameStateStorage>();
    auto sessions_storage = std::make_shared<model::SessionsStorage>(
        game_logic_manager, games_storage, game_state_storage);
    auto authentication_manager =
        std::make_shared<model::AuthenticationManager>();
    auto statistics_manager = std::make_shared<model::StatisticsManager>(
        sessions_storage, games_storage);

    // init controllers
    std::cout << "Initialize controllers..." << std::endl;
    auto games_controller =
        std::make_shared<controllers::GamesController>(games_storage);
    auto state_controller = std::make_shared<controllers::StateController>(
        games_storage, game_logic_manager, game_state_storage,
        sessions_storage);
    auto sessions_controller =
        std::make_shared<controllers::SessionsController>(
            games_storage, game_logic_manager, game_state_storage,
            sessions_storage, authentication_manager);
    auto statistics_controller =
        std::make_shared<controllers::StatisticsController>(games_storage,
                                                            statistics_manager);

    auto &app = drogon::app();

    // register controllers
    app.registerController(games_controller);
    app.registerController(state_controller);
    app.registerController(sessions_controller);
    app.registerController(statistics_controller);

    // print db and listeners on start
    app.registerBeginningAdvice([]() {
        auto dbclient = drogon::app().getDbClient();
        dbclient->connectionInfo();
        std::cout << "Database connection: "
                  << (dbclient == nullptr
                          ? "FAILED"
                          : hide_password(dbclient->connectionInfo()))
                  << std::endl;

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

    // start server
    app.run();
}
}  // namespace cavoke::server

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
    po::options_description desc("Allowed options");
    auto add_desc_options = desc.add_options();
    add_desc_options("help,h", "Print help");
    add_desc_options("config-file,c", po::value<std::string>(),
                     "File with game storage configuration");
    add_desc_options("port,p", po::value<uint16_t>()->default_value(8080),
                     "TCP/IP port number for connection");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }

    cavoke::server::model::GamesStorageConfig games_storage_config;
    if (vm.count("config-file")) {
        // load from file
        try {
            std::string file = vm["config-file"].as<std::string>();
            drogon::app().loadConfigFile(file);
            games_storage_config =
                nlohmann::to_nlohmann(drogon::app().getCustomConfig())
                    .get<cavoke::server::model::GamesStorageConfig>();
            std::cout << "Server configuration loaded from: '" << file << "'\n";
        } catch (const std::exception &err) {
            std::cerr
                << "Failed to load games storage configuration from file: "
                << err.what() << '\n';
            return 1;
        }
    } else {
        // use default one
        std::cerr
            << "No server configuration file specified. Using a default "
               "one.\nWARNING: No database requests will succeed.\nPlease use "
               "`-c` option to specify config file.\n";
        // default configuration
        games_storage_config = {"../../local_server/games", "logic",
                                "client.zip", "config.json"};
    }

    if (vm.count("port")) {
        uint16_t port = vm.at("port").as<uint16_t>();
        drogon::app().addListener("0.0.0.0", port);
    }

    cavoke::server::run(games_storage_config);
    return 0;
}
