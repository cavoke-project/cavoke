#include "cavoke.h"
#include <utility>

using json = nlohmann::json;

int main() {
    // supported commands
    // 1. VALIDATE <GameSettings>
    // 2. INIT <GameSettings>
    // 3. MOVE <GameMove>

    std::string command;
    std::cin >> command;

    std::string argument;
    char c;
    while (std::cin.get(c)) {
        argument.push_back(c);
    }

    std::cerr << "RECEIVED COMMAND: '" << command << ' ' << argument << "'"
              << std::endl;

    json argument_json = json::parse(argument);
    json result_json;

    if (command == "VALIDATE") {
        cavoke::GameSettings settings =
            argument_json.get<cavoke::GameSettings>();
        std::string message;
        bool success = cavoke::validate_settings(
            settings.settings, settings.occupied_positions,
            [&message](std::string message_) {
                message = std::move(message_);
            });
        result_json = cavoke::ValidationResult{success, message};
    } else if (command == "INIT") {
        cavoke::GameSettings settings =
            argument_json.get<cavoke::GameSettings>();
        result_json =
            cavoke::init_state(settings.settings, settings.occupied_positions);
    } else {
        cavoke::GameMove move = argument_json.get<cavoke::GameMove>();
        result_json = cavoke::apply_move(move);
    }

    std::cerr << "SEND RESULT: '" << result_json << "'" << std::endl;
    std::cout << result_json;
}
