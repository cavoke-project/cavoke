#ifndef CAVOKE_SERVER_GAME_STATE_STORAGE_H
#define CAVOKE_SERVER_GAME_STATE_STORAGE_H

#include <map>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

namespace cavoke::server::model {

class GameStateStorage {  // TODO: thread safety
public:
    struct GameState {
        bool is_terminal;
        std::string global_state;
        std::vector<std::string> players_state;
        std::vector<int> winners;
    };

    static GameState parse_state(const std::string &s);

    void save_state(const std::string &session_id, GameState new_state);

    std::optional<GameState> get_state(const std::string &session_id);

    std::optional<std::string> get_player_state(const std::string &session_id,
                                                int player_id);

private:
    std::map<std::string, GameState> m_states;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameStateStorage::GameState,
                                   is_terminal,
                                   global_state,
                                   players_state,
                                   winners);

}  // namespace cavoke::server::model

#endif  // CAVOKE_SERVER_GAME_STATE_STORAGE_H
