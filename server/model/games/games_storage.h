#ifndef CAVOKE_GAMES_STORAGE_H
#define CAVOKE_GAMES_STORAGE_H

#include <drogon/drogon.h>
#include <boost/filesystem/path.hpp>
#include <string>
#include "game.h"
#include "games_storage_config.h"
namespace cavoke::server::model {

class GamesStorage {
public:
    explicit GamesStorage(GamesStorageConfig config);

    void update();

    std::vector<Game> list_games();

    std::optional<Game> get_game_by_id(const std::string &game_id);

private:
    GamesStorageConfig m_config;
    std::map<std::string, Game> m_games;
    std::shared_mutex m_games_mtx;
};

}  // namespace cavoke::server::model

#endif  // CAVOKE_GAMES_STORAGE_H
