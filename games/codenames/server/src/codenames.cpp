#include "../../../cavoke.h"
#include "wordlists.h"

// max_players: 6
// 0 - blue captain
// 1 - red captain
// 2, 4 - blue team
// 3, 5 - red team
// requires 0, 1, (2 or 4), (3 or 5)
bool cavoke::validate_settings(
    const cavoke::json &settings,
    const std::vector<int> &occupied_positions,
    const std::function<void(std::string)> &message_callback) {
    if (!settings.contains("board_h") || !settings.contains("board_w") ||
        !settings.contains("word_list") ||
        settings["board_h"].get<int>() <= 5 ||
        settings["board_w"].get<int>() <= 4) {
        message_callback("Invalid config");
        return false;
    }

    try {
        codenames::WordList wl(settings["word_list"].get<std::string>());
    } catch (const codenames::WordList::wordlist_not_found &e) {
        message_callback(e.what());
        return false;
    }

    auto occupied_end = occupied_positions.end();
    if (std::find(occupied_positions.begin(), occupied_positions.end(), 0) ==
        occupied_end) {
        message_callback("No blue captain");
        return false;
    }

    if (std::find(occupied_positions.begin(), occupied_positions.end(), 1) ==
        occupied_end) {
        message_callback("No red captain");
        return false;
    }

    if (std::find(occupied_positions.begin(), occupied_positions.end(), 2) ==
            occupied_end &&
        std::find(occupied_positions.begin(), occupied_positions.end(), 4) ==
            occupied_end) {
        message_callback("No blue team");
        return false;
    }

    if (std::find(occupied_positions.begin(), occupied_positions.end(), 3) ==
            occupied_end &&
        std::find(occupied_positions.begin(), occupied_positions.end(), 5) ==
            occupied_end) {
        message_callback("No red team");
        return false;
    }

    return true;
}

cavoke::GameState cavoke::init_state(
    const cavoke::json &settings,
    const std::vector<int> &occupied_positions);

cavoke::GameState apply_move(cavoke::GameMove &new_move);
