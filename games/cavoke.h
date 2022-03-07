#ifndef CAVOKE_CAVOKE_H
#define CAVOKE_CAVOKE_H

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

// TODO: logging
namespace cavoke {
using json = nlohmann::json;

struct GameState {  // NOLINT(cppcoreguidelines-pro-type-member-init)
    bool is_terminal;
    std::string global_state;
    std::vector<std::string> players_state;
    std::vector<int> winners;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameState,
                                   is_terminal,
                                   global_state,
                                   players_state,
                                   winners)

struct GameMove {  // NOLINT(cppcoreguidelines-pro-type-member-init)
    int player_id;
    std::string move;
    std::string global_state;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameMove, player_id, move, global_state)

struct InitSettings {
    json settings;
    std::vector<int> occupied_positions;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(InitSettings,
                                   settings,
                                   occupied_positions);

struct ValidationResult {
    bool success;
    std::string message;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ValidationResult, success, message);

// called every time admin changes settings or new player arrives
// returns true if game can be started with current settings; false otherwise
// use message_callback(<some_message>) to display message to the players
bool validate_settings(
    const json &settings,
    const std::vector<int> &occupied_positions,
    const std::function<void(std::string)> &message_callback);

// called once when the game session is started
// generates initial state
// settings have passed the validation
GameState init_state(const json &settings,
                     const std::vector<int> &occupied_positions);

// applies move to the state
GameState apply_move(GameMove &new_move);

}  // namespace cavoke

#endif  // CAVOKE_CAVOKE_H
