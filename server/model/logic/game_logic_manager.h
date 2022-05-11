#ifndef CAVOKE_SERVER_GAME_LOGIC_MANAGER_H
#define CAVOKE_SERVER_GAME_LOGIC_MANAGER_H

#include <memory>
#include <string>
#include "cavoke.h"
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

    const int LOGIC_TIMEOUT_MS = 500;
    const int MAX_GAME_RESPONSE_B = 8 * 1024;

    ValidationResult validate_settings(
        const std::string &game_id,
        const json &settings,
        const std::vector<int> &occupied_positions);

    GameStateStorage::GameState init_state(
        const std::string &game_id,
        const json &settings,
        const std::vector<int> &occupied_positions);

    GameStateStorage::GameState send_move(const std::string &game_id,
                                          const cavoke::GameMove &move);
};

}  // namespace cavoke::server::model

#endif  // CAVOKE_SERVER_GAME_LOGIC_MANAGER_H
