#include "games_storage.h"
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <string>
#include <utility>

namespace cavoke::server::model {

GamesStorage::GamesStorage(GamesStorage::GamesStorageConfig config_)
    : m_config(std::move(config_)) {

  if (!boost::filesystem::exists(m_config.games_directory)) {
    bool success =
        boost::filesystem::create_directories(m_config.games_directory);
    if (!success) {
      throw std::invalid_argument("Cannot create games directory " +
                                  m_config.games_directory.string() + "\n");
    }
  }

  if (!boost::filesystem::is_directory(m_config.games_directory)) {
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
  m_games.clear();
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

std::optional<Game> GamesStorage::get_game_by_id(const std::string& game_id) {
  if (m_games.count(game_id) > 0) {
    return m_games[game_id];
  }
  return {};
}

} // namespace cavoke::server::model
