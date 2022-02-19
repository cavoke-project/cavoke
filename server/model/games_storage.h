#ifndef CAVOKE_GAMES_STORAGE_H
#define CAVOKE_GAMES_STORAGE_H

#include <drogon/drogon.h>
#include <boost/filesystem/path.hpp>
#include <string>
#include "GamesStorageConfig.h"
#include "game.h"
namespace cavoke::server::model {

class GamesStorage {
    // TODO: thread-safety
public:
    explicit GamesStorage(GamesStorageConfig config);

    void update();

    std::vector<Game> list_games();

    std::optional<Game> get_game_by_id(const std::string &game_id);

private:
    GamesStorageConfig m_config;
    std::map<std::string, Game> m_games;
};

}  // namespace cavoke::server::model

#endif  // CAVOKE_GAMES_STORAGE_H
