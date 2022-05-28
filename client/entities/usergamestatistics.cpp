#include "usergamestatistics.h"
#include <utility>

UserGameStatistics::UserGameStatistics() = default;

UserGameStatistics::UserGameStatistics(QString _game_id,
                                       int _time_played_sec,
                                       int _games_played,
                                       double _win_rate)
    : game_id(std::move(_game_id)),
      time_played_sec(_time_played_sec),
      games_played(_games_played),
      win_rate(_win_rate) {
}

void UserGameStatistics::read(const QJsonObject &json) {
    if (json.contains(GAME_ID) && json[GAME_ID].isString()) {
        game_id = json[GAME_ID].toString();
    }
    if (json.contains(TIME_PLAYED_SEC) && json[TIME_PLAYED_SEC].isDouble()) {
        time_played_sec = json[TIME_PLAYED_SEC].toInt();
    }
    if (json.contains(GAMES_PLAYED) && json[GAMES_PLAYED].isDouble()) {
        games_played = json[GAMES_PLAYED].toInt();
    }
    if (json.contains(WIN_RATE) && json[WIN_RATE].isDouble()) {
        win_rate = json[WIN_RATE].toDouble();
    }
}

void UserGameStatistics::write(QJsonObject &json) {
    assert(false);  // Should not be used
}
