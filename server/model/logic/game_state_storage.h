#ifndef CAVOKE_SERVER_GAME_STATE_STORAGE_H
#define CAVOKE_SERVER_GAME_STATE_STORAGE_H

#include <drogon/HttpAppFramework.h>
#include <drogon/orm/DbClient.h>
#include <map>
#include <mutex>
#include <nlohmann/json.hpp>
#include <optional>
#include <shared_mutex>
#include <string>
#include <vector>
#include "cavoke_base_exception.h"

namespace cavoke::server::model {

/// exception for errors thrown during actions with game states
struct game_state_error : cavoke_base_exception {
    explicit game_state_error(std::string message);
};

class GameStateStorage {
public:
    struct GameState {
        bool is_terminal;
        std::string global_state;
        std::vector<std::string> players_state;
        std::vector<int> winners;
    };

    void save_state(
        const std::string &session_id,
        GameStateStorage::GameState new_state,
        drogon::orm::DbClientPtr dbClient = drogon::app().getDbClient());

    GameState get_state(
        const std::string &session_id,
        drogon::orm::DbClientPtr dbClient = drogon::app().getDbClient());

    std::string get_player_state(const std::string &session_id, int player_id);
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameStateStorage::GameState,
                                   is_terminal,
                                   global_state,
                                   players_state,
                                   winners);

}  // namespace cavoke::server::model

#endif  // CAVOKE_SERVER_GAME_STATE_STORAGE_H
