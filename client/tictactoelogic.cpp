#include "tictactoelogic.h"
#include <sstream>

namespace tictactoe {

bool winner(std::vector<char> board) {
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

void tictactoelogic::restartGame() {
    running = true;
    for (int i = 0; i < 9; ++i) {
        board[i] = ' ';
    }
}

bool tictactoelogic::makeMove(int pos, char player) {
    board[pos] = player;
    if (winner(board)) {
        // gameFinished(player + " wins")
        return true;
    }
    return false;
}

bool tictactoelogic::canPlayAtPos(int pos) {
    return board[pos] == ' ';
}

void tictactoelogic::computerTurn() {
    double r = dis(gen);
    if (r < difficulty) {
        smartAI();
    } else {
        randomAI();
    }
}

void tictactoelogic::smartAI() {
    for (int i = 0; i < 9; ++i) {
        std::vector<char> simpleBoard = board;
        if (canPlayAtPos(i)) {
            simpleBoard[i] = 'X';
            if (winner(simpleBoard)) {
                makeMove(i, 'O');
                return;
            }
        }
    }

    auto thwart = [this](int a, int b, int c) {
        if (board[a] == 'X') {
            if (canPlayAtPos(b)) {
                makeMove(b, 'O');
                return true;
            } else if (canPlayAtPos(c)) {
                makeMove(c, 'O');
                return true;
            }
        }
        return false;
    };

    if (thwart(4, 0, 2))
        return;
    if (thwart(0, 4, 3))
        return;
    if (thwart(2, 4, 1))
        return;
    if (thwart(6, 4, 7))
        return;
    if (thwart(8, 4, 5))
        return;
    if (thwart(1, 4, 2))
        return;
    if (thwart(3, 4, 0))
        return;
    if (thwart(5, 4, 8))
        return;
    if (thwart(7, 4, 6))
        return;

    for (int i = 0; i < 9; i++) {
        if (canPlayAtPos(i)) {
            makeMove(i, 'O');
            return;
        }
    }
    restartGame();
}

void tictactoelogic::randomAI() {
    std::vector<int> unfilledPosns;

    for (int i = 0; i < 9; ++i) {
        if (canPlayAtPos(i)) {
            unfilledPosns.emplace_back(i);
        }
    }

    if (unfilledPosns.empty()) {
        restartGame();
    } else {
        int choice = unfilledPosns[std::floor(dis(gen) * unfilledPosns.size())];
        makeMove(choice, 'O');
    }
}
QString tictactoelogic::get_board_as_string() {
    return QString::fromStdString(
        std::string(board.begin(), board.end()));  // FIXME: sounds bad
}
QString tictactoelogic::processAction(const QString &jsonMove) {
    char action;
    std::stringstream to_split(jsonMove.toStdString());
    to_split >> action;
    if (action == 'D') {
        double new_d;
        to_split >> new_d;
        if (new_d >= 0.0 && new_d <= 1.0) {
            difficulty = new_d;
            return "Difficulty changed";
        }
    } else {
        int pos;
        to_split >> pos;
        if (!(pos >= 0 && pos < 9 && canPlayAtPos(pos))) {
            return "Invalid action";
        }
        makeMove(pos, 'X');
        if (winner(board)) {
            running = false;
            return "X wins";
        }
        computerTurn();
        if (winner(board)) {
            running = false;
            return "O wins";
        }
        return "Nothing or draw";
    }
    return "Invalid action";
}

}  // namespace tictactoe
