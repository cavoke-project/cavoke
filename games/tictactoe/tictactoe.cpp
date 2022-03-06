#include <sstream>
#include "../cavoke.h"

namespace cavoke {
const int BOARD_SIZE = 3 * 3;

char current_player(std::string &board) {
    int xs_cnt = 0;
    int os_cnt = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
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
    return board[position] == ' ';
}

bool winner(const std::string &board) {
    for (int i = 0; i < 3; ++i) {
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

GameState init_state() {
    std::string board(BOARD_SIZE, ' ');

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
