#ifndef CAVOKE_SERVER_GAME_LOGIC_MANAGER_H
#define CAVOKE_SERVER_GAME_LOGIC_MANAGER_H

#include <memory>
#include <string>
#include "game_state_storage.h"
#include "games_storage.h"

namespace cavoke::server::model {

class GameLogicManager {
    std::shared_ptr<GamesStorage> m_games_storage;

    struct InitSettings {
        json settings;
        std::vector<int> occupied_positions;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(InitSettings,
                                       settings,
                                       occupied_positions);

    std::string invoke_logic(const Game &game, const std::string &input);

public:
    explicit GameLogicManager(std::shared_ptr<GamesStorage> games_storage);

    struct GameMove {
        int player_id;
        std::string move;
        std::string global_state;

        [[nodiscard]] Json::Value to_json() const;
    };
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameMove, player_id, move, global_state);

    bool validate_settings(const std::string &game_id,
                           const json &settings,
                           const std::vector<int> &occupied_positions,
                           std::string &error_message);

    GameStateStorage::GameState init_state(
        const std::string &game_id,
        const json &settings,
        const std::vector<int> &occupied_positions);

    GameStateStorage::GameState send_move(const std::string &game_id,
                                          const GameMove &move);
};

}  // namespace cavoke::server::model

#endif  // CAVOKE_SERVER_GAME_LOGIC_MANAGER_H
