#ifndef CAVOKE_CAVOKE_H
#define CAVOKE_CAVOKE_H

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace cavoke {

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

GameState init_state();
GameState apply_move(GameMove &new_move);

// TODO: logging

}  // namespace cavoke

using json = nlohmann::json;

int main() {
    std::string input;
    char c;
    while (std::cin.get(c)) {
        input.push_back(c);
    }
    std::cerr << "RECEIVED MOVE: '" << input << "'" << std::endl;
    json input_json = json::parse(input);
    cavoke::GameMove move = input_json.get<cavoke::GameMove>();

    cavoke::GameState new_state;
    if (move.player_id == -1) {
        new_state = cavoke::init_state();
    } else {
        new_state = cavoke::apply_move(move);
    }

    json result_json = new_state;

    std::cerr << "SEND RESULT: '" << result_json << "'" << std::endl;
    std::cout << result_json;
}

#endif  // CAVOKE_CAVOKE_H
