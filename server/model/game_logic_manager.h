#ifndef CAVOKE_SERVER_GAME_LOGIC_MANAGER_H
#define CAVOKE_SERVER_GAME_LOGIC_MANAGER_H

#include <memory>
#include <string>
#include "game_state_storage.h"
#include "games_storage.h"

namespace cavoke::server::model {

class GameLogicManager {
    std::shared_ptr<GamesStorage> m_games_storage;

public:
    explicit GameLogicManager(std::shared_ptr<GamesStorage> games_storage);
    // TODO: rename to GameMove
    struct GameUpdate {
        int player_id;
        std::string update;
        std::string global_state;

        [[nodiscard]] Json::Value to_json() const;
    };

    GameStateStorage::GameState send_update(const std::string &game_id,
                                            const GameUpdate &update);
};

}  // namespace cavoke::server::model

#endif  // CAVOKE_SERVER_GAME_LOGIC_MANAGER_H
