#include "game.h"
#include "games_storage.h"
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <string>
#include <utility>

namespace cavoke::server::model {

Game::Game(boost::filesystem::path directory,
           const GamesStorageConfig &game_storage_config)
    : directory(std::move(directory)), logic_file(directory / LOGIC_FILE),
      client_file(directory / CLIENT_FILE),
      CLIENT_FILE(game_storage_config.zip_name),
      LOGIC_FILE(game_storage_config.logic_name),
      CONFIG_FILE(game_storage_config.json_name) {
  Json::Value json_config;
  read_config_file(directory / CONFIG_FILE, json_config);
  config.id = json_config["id"].asString();
  config.display_name = json_config["display_name"].asString();
  config.description = json_config["description"].asString();
  config.players_num = json_config["players_num"].asInt();
}

bool Game::is_game_directory(const boost::filesystem::path &path,
                             const GamesStorageConfig &games_storage_config) {
  Json::Value tmp;
  // TODO: rewrite
  const auto& CLIENT_FILE = games_storage_config.zip_name,
             CONFIG_FILE = games_storage_config.json_name,
             LOGIC_FILE = games_storage_config.logic_name;
  // TODO: extract function exists & directory + exists & regular file
  return boost::filesystem::exists(path) &&
         boost::filesystem::is_directory(path) &&
         boost::filesystem::exists(path / CLIENT_FILE) &&
         boost::filesystem::is_regular_file(path / CLIENT_FILE) &&
         boost::filesystem::exists(path / CONFIG_FILE) &&
         boost::filesystem::is_regular_file(path / CONFIG_FILE) &&
         boost::filesystem::exists(path / LOGIC_FILE) &&
         boost::filesystem::is_regular_file(path / LOGIC_FILE) &&
         read_config_file(path / CONFIG_FILE, tmp);
}

bool Game::read_config_file(const boost::filesystem::path &path,
                            Json::Value &json_obj) {
  std::ifstream file(path);
  Json::Reader reader; // deprecated
  bool success = reader.parse(file, json_obj, false);
  file.close();
  return success;
}

Json::Value Game::GameConfig::to_json() const {
  // TODO: please let's use https://github.com/nlohmann/json#arbitrary-types-conversions
  Json::Value result;
  result["id"] = id;
  result["display_name"] = display_name;
  result["description"] = description;
  result["players_num"] = players_num;

  return result;
}

} // namespace cavoke::server::model
