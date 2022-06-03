#include "cavoke.h"
#include "model.h"
#include "wordlists.h"

const int MAX_PLAYERS = 6;
const std::vector<int> BLUE_PLAYERS = {0, 2, 4};  // NOLINT(cert-err58-cpp)
const std::vector<int> RED_PLAYERS = {1, 3, 5};   // NOLINT(cert-err58-cpp)

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
        !settings.contains("word_list") || settings["board_h"].get<int>() < 5 ||
        settings["board_w"].get<int>() < 4) {
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

cavoke::GameState model_to_game_state(const codenames::CodenamesModel &model) {
    cavoke::GameState result;
    result.is_terminal =
        (model.result() != codenames::CodenamesModel::GAME_RESULT::IN_PROGRESS);

    if (result.is_terminal) {
        if (model.result() ==
            codenames::CodenamesModel::GAME_RESULT::BLUE_WINS) {
            result.winners = BLUE_PLAYERS;
        } else {
            result.winners = RED_PLAYERS;
        }
    }

    result.global_state = model.get_global_state();
    result.players_state.resize(MAX_PLAYERS);

    for (int i = 0; i < MAX_PLAYERS; ++i) {
        result.players_state[i] = model.get_player_state(i);
    }

    return result;
}

cavoke::GameState cavoke::init_state(
    const cavoke::json &settings,
    const std::vector<int> &occupied_positions) {
    codenames::CodenamesModel model(
        settings["board_h"].get<int>(), settings["board_w"].get<int>(),
        codenames::WordList(settings["word_list"].get<std::string>()));

    return model_to_game_state(model);
}

bool is_right_player(const codenames::CodenamesModel &model, int player_id) {
    return ((player_id == 2 || player_id == 4) &&
            model.stage() ==
                codenames::CodenamesModel::GAME_STAGE::BLUE_TEAM) ||
           ((player_id == 3 || player_id == 5) &&
            model.stage() == codenames::CodenamesModel::GAME_STAGE::RED_TEAM);
}

bool is_right_captain(const codenames::CodenamesModel &model, int player_id) {
    return (player_id == 0 &&
            model.stage() ==
                codenames::CodenamesModel::GAME_STAGE::BLUE_CAPTAIN) ||
           (player_id == 1 &&
            model.stage() ==
                codenames::CodenamesModel::GAME_STAGE::RED_CAPTAIN);
}

cavoke::GameState cavoke::apply_move(cavoke::GameMove &new_move) {
    codenames::CodenamesModel model =
        nlohmann::json::parse(new_move.global_state);
    auto command = nlohmann::json::parse(new_move.move);
    auto type = command["type"].get<std::string>();

    if (type == "hint") {
        auto hint = command["hint"].get<std::string>();
        auto attempts = command["attempts"].get<int>();

        if (is_right_captain(model, new_move.player_id)) {
            model.make_hint(hint, attempts);
        }
    } else if (type == "open") {
        auto pos = command["position"].get<int>();
        if (is_right_player(model, new_move.player_id)) {
            model.open_card(pos);
        }
    } else if (type == "skip") {
        if (is_right_player(model, new_move.player_id)) {
            model.skip();
        }
    }

    return model_to_game_state(model);
}
