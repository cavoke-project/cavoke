#include "game_state_storage.h"
#include <json/reader.h>
#include <utility>

namespace cavoke::server::model {

using json = nlohmann::json;

void GameStateStorage::save_state(const std::string &session_id,
                                  GameStateStorage::GameState new_state) {
    std::unique_lock lock(m_states_mtx);
    m_states[session_id] = std::move(new_state);
}

GameStateStorage::GameState GameStateStorage::get_state(
    const std::string &session_id) {
    std::shared_lock lock(m_states_mtx);
    if (m_states.count(session_id) == 0) {
        throw game_state_error("No state for session '" + session_id + "'");
    }
    return m_states[session_id];
}

std::string GameStateStorage::get_player_state(const std::string &session_id,
                                               int player_id) {
    GameState state = get_state(session_id);

    if (player_id >= state.players_state.size()) {
        throw game_state_error("There's no player '" +
                               std::to_string(player_id) + "' in session '" +
                               session_id + "'");
    }

    return state.players_state[player_id];
}

game_state_error::game_state_error(std::string message)
    : cavoke_base_exception(std::move(message), NotFound, "cavoke/state") {
}

}  // namespace cavoke::server::model
