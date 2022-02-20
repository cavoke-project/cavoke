#ifndef CAVOKE_GAMES_STORAGE_H
#define CAVOKE_GAMES_STORAGE_H

#include "game.h"
#include <boost/filesystem/path.hpp>
#include <drogon/drogon.h>
#include <string>

namespace cavoke::server::model {

class GamesStorage {
  // TODO: thread-safety
public:
  struct GamesStorageConfig {
    boost::filesystem::path games_directory;
  };

  explicit GamesStorage(GamesStorageConfig config);

  void update();

  std::vector<Game> list_games();

  std::optional<Game> get_game_by_id(const std::string &game_id);

private:
  GamesStorageConfig m_config;
  std::map<std::string, Game> m_games;
};

} // namespace cavoke::server::model

#endif // CAVOKE_GAMES_STORAGE_H
