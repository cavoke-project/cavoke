#include <sstream>
#include "../cavoke.h"

namespace cavoke {

bool validate_settings(
    const json &settings,
    const std::vector<int> &occupied_positions,
    const std::function<void(std::string)> &message_callback) {
    if (occupied_positions.size() != 2) {
        message_callback("Not enough players");
        return false;
    }

    if (!settings.contains("board_size")) {
        message_callback("No bord_size property");
        return false;
    }

    if (settings["board_size"].get<int>() != 3 &&
        settings["board_size"].get<int>() != 5) {
        message_callback("Only 3 and 5 board_size values are supported");
        return false;
    }

    return true;
}

int get_board_size(const std::string &board) {
    return static_cast<int>(sqrt(static_cast<double>(board.size())));
}

char current_player(std::string &board) {
    int xs_cnt = 0;
    int os_cnt = 0;
    for (int i = 0; i < get_board_size(board); ++i) {
        if (board[i] == 'X') {
            xs_cnt++;
        } else if (board[i] == 'O') {
            os_cnt++;
        }
    }

    return (xs_cnt == os_cnt ? 'X' : 'O');
}

int extract_position(std::string &move) {
    std::stringstream to_split(move);
    char action;
    to_split >> action;
    int position;
    to_split >> position;
    return position;
};

bool is_valid_move(std::string &board, int position) {
    return position > 0 && position < board.size() && board[position] == ' ';
}

// TODO: fix for 5x5 board
bool winner(const std::string &board) {
    for (int i = 0; i < get_board_size(board); ++i) {
        if (board[i] != ' ' && board[i] == board[i + 3] &&
            board[i] == board[i + 6])
            return true;

        if (board[i * 3] != ' ' && board[i * 3] == board[i * 3 + 1] &&
            board[i * 3] == board[i * 3 + 2])
            return true;
    }

    if (board[0] != ' ' && board[0] == board[4] && board[0] == board[8])
        return true;

    if (board[2] != ' ' && board[2] == board[4] && board[2] == board[6])
        return true;

    return false;
}

GameState init_state(const json &settings,
                     const std::vector<int> &occupied_positions) {
    int board_size = settings["board_size"];
    std::string board(board_size * board_size, ' ');

    return GameState{false, board, {board, board}, {}};
}

GameState apply_move(GameMove &new_move) {
    std::string &board = new_move.global_state;
    char player = (new_move.player_id == 0 ? 'X' : 'O');

    if (player != current_player(board)) {
        return {false, board, {board, board}, {}};
    }

    int position = extract_position(new_move.move);

    if (!is_valid_move(board, position)) {
        return {false, board, {board, board}, {}};
    }

    board[position] = player;
    bool win = winner(board);

    std::vector<int> winners;
    if (win) {
        winners.push_back(new_move.player_id);
    }

    return {win, board, {board, board}, winners};
}
}  // namespace cavoke
