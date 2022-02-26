#include "game_logic_manager.h"
#include <utility>
#include "mock_tictactoe/tictactoe.h"

namespace cavoke::server::model {

GameLogicManager::GameLogicManager(std::shared_ptr<GamesStorage> games_storage)
    : m_games_storage(std::move(games_storage)) {
}

GameStateStorage::GameState GameLogicManager::send_move(
    const std::string &game_id,
    const GameLogicManager::GameMove &move) {
    // TODO: invoke actual logic
    GameStateStorage::GameState result = GameStateStorage::parse_state(
        tictactoe::apply(move.to_json().toStyledString()));
    return result;
}

Json::Value GameLogicManager::GameMove::to_json() const {
    Json::Value result;
    result["player_id"] = player_id;
    result["update"] = move;
    result["global_state"] = global_state;

    return result;
}
}  // namespace cavoke::server::model
