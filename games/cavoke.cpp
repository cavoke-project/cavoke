#include "cavoke.h"

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
