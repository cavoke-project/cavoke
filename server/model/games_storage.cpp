#include "games_storage.h"
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>

namespace cavoke::server::model {

GamesStorage::GamesStorage(GamesStorage::GamesStorageConfig config_)
    : m_config(std::move(config_)) {

  if (!boost::filesystem::exists(m_config.games_directory) ||
      !boost::filesystem::is_directory(m_config.games_directory)) {
    throw std::invalid_argument("Invalid games directory \"" +
                                m_config.games_directory.string() + "\"\n");
  }

  for (auto &entry : boost::make_iterator_range(
           boost::filesystem::directory_iterator(m_config.games_directory),
           {})) {
    if (!Game::is_game_directory(entry)) {
      // TODO: logging
      std::cerr << "\"" << entry
                << "\" from games directory is not a valid game" << std::endl;
    } else {
      Game game(entry);
      m_games[game.config.id] = game;
    }
  }
}

std::vector<Game> GamesStorage::list_games() {
  std::vector<Game> result;
  for (auto e : m_games) {
    result.emplace_back(e.second);
  }
  return result;
}

const std::string Game::CONFIG_FILE = "m_config.json"; // NOLINT(cert-err58-cpp)
const std::string Game::CLIENT_FILE = "client.zip";    // NOLINT(cert-err58-cpp)
const std::string Game::LOGIC_FILE = "logic";          // NOLINT(cert-err58-cpp)

Game::Game(boost::filesystem::path directory_)
    : directory(std::move(directory_)), logic_file(directory / LOGIC_FILE),
      client_file(directory / CLIENT_FILE) {
  nlohmann::json json_config = read_config_file(directory / CONFIG_FILE);
  config.id = json_config["config"];
  config.display_name = json_config["display_name"];
  config.description = json_config["description"];
  config.players_num = json_config["players_num"];
}

bool Game::is_game_directory(const boost::filesystem::path &directory) {
  return boost::filesystem::exists(directory) &&
         boost::filesystem::is_directory(directory) &&
         boost::filesystem::exists(directory / CLIENT_FILE) &&
         boost::filesystem::is_regular_file(directory / CLIENT_FILE) &&
         boost::filesystem::exists(directory / CONFIG_FILE) &&
         boost::filesystem::is_regular_file(directory / CONFIG_FILE) &&
         boost::filesystem::exists(directory / LOGIC_FILE) &&
         boost::filesystem::is_regular_file(directory / LOGIC_FILE) &&
         is_valid_config_file(directory / CONFIG_FILE);
}
bool Game::is_valid_config_file(const boost::filesystem::path &path) {
  try {
    read_config_file(path);
  } catch (nlohmann::json::parse_error &e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  return true;
}

nlohmann::json Game::read_config_file(const boost::filesystem::path &path) {
  std::ifstream file(path);
  nlohmann::json config;
  file >> config;
  return config;
}
} // namespace cavoke::server::model
