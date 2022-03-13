#include "games_storage_config.h"
#include <boost/filesystem.hpp>
#include <iostream>

namespace cavoke::server::model {

GamesStorageConfig GamesStorageConfig::load(const std::string &config_file) {
    if (config_file.empty()) {
        // FIXME: rewrite this mess...
        std::cerr << "No games storage config specified. Using a default one. "
                     "Please use `-c` option.\n";
        return GamesStorageConfig{"../../local_server/games", "logic",
                                  "client.zip", "config.json"};
    }
    if (!boost::filesystem::exists(config_file) ||
        !boost::filesystem::is_regular_file(config_file)) {  // FIXME
        throw std::invalid_argument(
            "No such game storage configuration file: " + config_file);
    }

    std::ifstream file(config_file);
    json j = json::parse(file);
    return j.get<GamesStorageConfig>();
}

}  // namespace cavoke::server::model