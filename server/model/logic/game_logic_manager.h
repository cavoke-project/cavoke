#ifndef CAVOKE_SERVER_GAME_LOGIC_MANAGER_H
#define CAVOKE_SERVER_GAME_LOGIC_MANAGER_H

#include <memory>
#include <string>
#include "game_state_storage.h"
#include "model/games/games_storage.h"

namespace cavoke::server::model {

/// is thrown when trying to start a new session with invalid config
struct validation_error : cavoke_base_exception {
    explicit validation_error(std::string message);
};

class GameLogicManager {
    std::shared_ptr<GamesStorage> m_games_storage;

    std::string invoke_logic(const Game &game, const std::string &input);

public:
    explicit GameLogicManager(std::shared_ptr<GamesStorage> games_storage);

    struct GameMove {
        int player_id;
        std::string move;
        std::string global_state;
    };

    struct InitSettings {
        json settings;
        std::vector<int> occupied_positions;
    };

    struct ValidationResult {
        bool success;
        std::string message;
    };

    ValidationResult validate_settings(
        const std::string &game_id,
        const json &settings,
        const std::vector<int> &occupied_positions);

    GameStateStorage::GameState init_state(
        const std::string &game_id,
        const json &settings,
        const std::vector<int> &occupied_positions);

    GameStateStorage::GameState send_move(const std::string &game_id,
                                          const GameMove &move);
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameLogicManager::InitSettings,
                                   settings,
                                   occupied_positions);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameLogicManager::GameMove,
                                   player_id,
                                   move,
                                   global_state);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameLogicManager::ValidationResult,
                                   success,
                                   message);

}  // namespace cavoke::server::model

#endif  // CAVOKE_SERVER_GAME_LOGIC_MANAGER_H