#include "GamesStorageConfig.h"
#include <boost/filesystem.hpp>
#include <json/reader.h>
#include <iostream>

namespace cavoke::server::model {

GamesStorageConfig
GamesStorageConfig::load(const std::string &config_file) {
  if (config_file.empty()) {
    // FIXME: rewrite this mess...
    std::cerr << "No games storage config specified. Using a default one. Please use `-c` option.\n";
    return GamesStorageConfig{
        "../../local_server/games",
        "logic",
        "client.zip",
        "config.json"
    };
  }
  if (!boost::filesystem::exists(config_file) ||
      !boost::filesystem::is_regular_file(config_file)) { // FIXME
    throw std::invalid_argument("No such game storage configuration file: " + config_file);
  }

  // FIXME: trash code; let's use https://github.com/nlohmann/json#arbitrary-types-conversions or something similar
  Json::Value obj;
  std::ifstream file(config_file);
  Json::Reader reader;
  assert(reader.parse(file, obj, false));
  file.close();

  GamesStorageConfig res{obj["games_directory"].as<std::string>(), obj["logic_name"].as<std::string>(), obj["zip_name"].as<std::string>(), obj["config_name"].as<std::string>()};
  return res;
}

}