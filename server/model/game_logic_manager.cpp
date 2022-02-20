#include "game_logic_manager.h"
#include "mock_tictactoe/tictactoe.h"

#include <utility>

namespace cavoke::server::model {

GameLogicManager::GameLogicManager(std::shared_ptr<GamesStorage> games_storage)
    : m_games_storage(std::move(games_storage)) {}

GameStateStorage::GameState
GameLogicManager::send_update(const std::string &game_id,
                              const GameLogicManager::GameUpdate &update) {
  // TODO: invoke actual logic
  GameStateStorage::GameState result = GameStateStorage::parse_state(
      tictactoe::apply(update.to_json().toStyledString()));
  return result;
}

Json::Value GameLogicManager::GameUpdate::to_json() const {
  Json::Value result;
  result["player_id"] = player_id;
  result["update"] = update;
  result["global_state"] = global_state;

  return result;
}
} // namespace cavoke::server::model
