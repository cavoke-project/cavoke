#include "games_storage.h"
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <string>
#include <utility>

namespace cavoke::server::model {

GamesStorage::GamesStorage(GamesStorage::GamesStorageConfig config_)
    : m_config(std::move(config_)) {

  if (!boost::filesystem::exists(m_config.games_directory) ||
      !boost::filesystem::is_directory(m_config.games_directory)) {
    throw std::invalid_argument("Invalid games directory " +
                                m_config.games_directory.string() + "\n");
  }

  update();
}

std::vector<Game> GamesStorage::list_games() {
  update();

  std::vector<Game> result;
  for (auto e : m_games) {
    result.emplace_back(e.second);
  }
  return result;
}

void GamesStorage::update() {
  for (auto &entry : boost::make_iterator_range(
           boost::filesystem::directory_iterator(m_config.games_directory),
           {})) {
    if (!Game::is_game_directory(entry)) {
      // TODO: logging
      std::cerr << entry << " entity from games directory is not a valid game"
                << std::endl;
    } else {
      Game game(entry);
      m_games.emplace(game.config.id, game);
    }
  }
}

const std::string Game::CONFIG_FILE = "config.json"; // NOLINT(cert-err58-cpp)
const std::string Game::CLIENT_FILE = "client.zip";  // NOLINT(cert-err58-cpp)
const std::string Game::LOGIC_FILE = "logic";        // NOLINT(cert-err58-cpp)

Game::Game(boost::filesystem::path directory_)
    : directory(std::move(directory_)), logic_file(directory / LOGIC_FILE),
      client_file(directory / CLIENT_FILE) {
  Json::Value json_config;
  read_config_file(directory / CONFIG_FILE, json_config);
  config.id = json_config["config"].asString();
  config.display_name = json_config["display_name"].asString();
  config.description = json_config["description"].asString();
  config.players_num = json_config["players_num"].asInt();
}

bool Game::is_game_directory(const boost::filesystem::path &directory) {
  Json::Value tmp;
  return boost::filesystem::exists(directory) &&
         boost::filesystem::is_directory(directory) &&
         boost::filesystem::exists(directory / CLIENT_FILE) &&
         boost::filesystem::is_regular_file(directory / CLIENT_FILE) &&
         boost::filesystem::exists(directory / CONFIG_FILE) &&
         boost::filesystem::is_regular_file(directory / CONFIG_FILE) &&
         boost::filesystem::exists(directory / LOGIC_FILE) &&
         boost::filesystem::is_regular_file(directory / LOGIC_FILE) &&
         read_config_file(directory / CONFIG_FILE, tmp);
}

bool Game::read_config_file(const boost::filesystem::path &path,
                            Json::Value &json_obj) {
  std::ifstream file(path);
  Json::Reader reader;
  bool success = reader.parse(file, json_obj, false);
  file.close();
  return success;
}

Json::Value Game::GameConfig::to_json() const {
  Json::Value result;
  result["id"] = id;
  result["display_name"] = display_name;
  result["description"] = description;
  result["players_num"] = players_num;

  return result;
}

} // namespace cavoke::server::model
