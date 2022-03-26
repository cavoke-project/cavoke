#include <drogon/HttpAppFramework.h>
#include <boost/program_options.hpp>
#include "controllers/games_controller.h"
#include "controllers/sessions_controller.h"
#include "controllers/state_controller.h"
#include "model/games/games_storage.h"
#include "model/logic/game_logic_manager.h"
#include "model/sessions/sessions_storage.h"

namespace cavoke::server {
void run(const std::string &host,
         uint16_t port,
         const model::GamesStorageConfig &games_storage_config) {
    // TODO: logging

    // init models
    std::cout << "Initialize models..." << std::endl;
    auto games_storage =
        std::make_shared<model::GamesStorage>(games_storage_config);
    auto game_logic_manager =
        std::make_shared<model::GameLogicManager>(games_storage);
    auto game_state_storage = std::make_shared<model::GameStateStorage>();
    auto participation_storage = std::make_shared<model::SessionsStorage>(
        game_logic_manager, games_storage, game_state_storage);
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
    auto add_desc_options = desc.add_options();
    add_desc_options("help,h", "Print help");
    add_desc_options("config-file,c", po::value<std::string>(),
                     "File with game storage configuration");
    add_desc_options(
        "config,s", po::value<std::string>(),
        "Game storage configuration as json (overrides config-file)");
    add_desc_options("games-dir,g", po::value<std::string>(),
                     "Overrides game directory location (both of config and "
                     "config-file). Used for mounting.");
    add_desc_options("host,a",
                     po::value<std::string>()->default_value("0.0.0.0"),
                     "Host on which server is located");
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
    if (vm.count("config")) {
        // load from argument
        try {
            games_storage_config =
                nlohmann::json::parse(vm["config"].as<std::string>());
            std::cout << "Games storage config loaded from an argument\n";
        } catch (const std::exception &err) {
            std::cerr << "Failed to parse games storage configuration: "
                      << err.what() << '\n';
            return 1;
        }
    } else if (vm.count("config-file")) {
        // load from file
        try {
            std::string file = vm["config-file"].as<std::string>();
            games_storage_config =
                cavoke::server::model::GamesStorageConfig::load(file);
            std::cout << "Games storage config loaded from file: '" << file
                      << "'\n";
        } catch (const std::exception &err) {
            std::cerr
                << "Failed to load games storage configuration from file: "
                << err.what() << '\n';
            return 1;
        }
    } else {
        // use default one
        std::cerr << "No games storage config specified. Using a default one. "
                     "Please use `-c` option.\n";
        // default configuration
        games_storage_config = {"../../local_server/games", "logic",
                                "client.zip", "config.json"};
    }
    // try override games storage
    if (vm.count("games-dir")) {
        games_storage_config.games_directory =
            vm["games-dir"].as<std::string>();
    }

    std::string host = vm.at("host").as<std::string>();
    uint16_t port = vm.at("port").as<uint16_t>();

    cavoke::server::run(host, port, games_storage_config);
    return 0;
}
