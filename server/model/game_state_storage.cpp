#include "game_state_storage.h"

#include <json/reader.h>
#include <utility>

namespace cavoke::server::model {

void GameStateStorage::save_state(const std::string &session_id,
                                  GameStateStorage::GameState new_state) {
  m_states[session_id] = std::move(new_state);
}

std::optional<GameStateStorage::GameState>
GameStateStorage::get_state(const std::string &session_id) {
  if (m_states.count(session_id) == 0) {
    return {};
  }
  return m_states[session_id];
}

GameStateStorage::GameState
GameStateStorage::parse_state(const std::string &s) {
  GameState state;
  Json::Value json_state;
  Json::Reader reader;
  reader.parse(s, json_state, false);
  state.is_terminal = json_state["is_terminal"].asBool();
  state.global_state = json_state["global_state"].asString();
  for (Json::Value::ArrayIndex i = 0; i != json_state["players_state"].size();
       i++) {
    state.players_state.push_back(json_state["players_state"][i].asString());
  }
  for (Json::Value::ArrayIndex i = 0; i != json_state["winners"].size(); i++) {
    state.winners.push_back(json_state["winners"][i].asInt());
  }
  return state;
}

} // namespace cavoke::server::model
