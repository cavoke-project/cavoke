#include "tictactoe.h"
#include <cmath>
#include <json/reader.h>
#include <random>
#include <sstream>

namespace tictactoe {

std::mt19937 gen;
std::uniform_real_distribution<> dis;

bool winner(std::string board) {
  for (int i = 0; i < 3; ++i) {
    if (board[i] != ' ' && board[i] == board[i + 3] && board[i] == board[i + 6])
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

bool makeMove(int pos, char player, std::string &board) {
  board[pos] = player;
  if (winner(board)) {
    // gameFinished(player + " wins")
    return true;
  }
  return false;
}

void restartGame(std::string &board) {
  for (int i = 0; i < 9; ++i) {
    board[i] = ' ';
  }
}

bool canPlayAtPos(int pos, std::string &board) { return board[pos] == ' '; }

void randomAI(std::string &board) {
  std::vector<int> unfilledPosns;

  for (int i = 0; i < 9; ++i) {
    if (canPlayAtPos(i, board)) {
      unfilledPosns.emplace_back(i);
    }
  }

  if (unfilledPosns.empty()) {
    restartGame(board);
  } else {
    int choice = unfilledPosns[std::floor(dis(gen) * unfilledPosns.size())];
    makeMove(choice, 'O', board);
  }
}

std::string apply(const std::string &request) {
  Json::Reader reader;
  Json::Value json_request;
  reader.parse(request, json_request, false);
  std::string board = json_request["global_state"].asString();
  std::string update = json_request["update"].asString();
  int player_id = json_request["player_id"].asInt();

  std::string new_global_state;
  std::string new_player_state;

  std::string message = "...";

  // player_id == -1 -> start new game
  if (player_id == -1) {
    board = std::string(9, ' ');
  } else {
    std::stringstream to_split(update);
    char action;
    to_split >> action;
    if (action != 'D') {
      int pos;
      to_split >> pos;
      makeMove(pos, 'X', board);
      if (winner(board)) {
        message = "X wins";
        restartGame(board);
      } else {
        randomAI(board);
        if (winner(board)) {
          message = "O wins";
          restartGame(board);
        }
      }
    }
  }

  std::string new_state = message + "\n" + board;

  Json::Value json_result;
  json_result["is_terminal"] = false;
  json_result["global_state"] = board;
  json_result["players_state"].append(board);
  json_result["winners"] = Json::arrayValue;

  return json_result.asString();
}

} // namespace tictactoe
