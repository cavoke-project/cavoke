#include "userstatistics.h"

UserStatistics::UserStatistics() = default;

UserStatistics::UserStatistics(int _total_time_played_sec,
                               int _total_games_played)
    : total_time_played_sec(_total_time_played_sec),
      total_games_played(_total_games_played) {
}

void UserStatistics::read(const QJsonObject &json) {
    if (json.contains(TOTAL_TIME_PLAYED_SEC) &&
        json[TOTAL_TIME_PLAYED_SEC].isDouble()) {
        total_time_played_sec = json[TOTAL_TIME_PLAYED_SEC].toInt();
    }
    if (json.contains(TOTAL_GAMES_PLAYED) &&
        json[TOTAL_GAMES_PLAYED].isDouble()) {
        total_games_played = json[TOTAL_GAMES_PLAYED].toInt();
    }
}

void UserStatistics::write(QJsonObject &json) {
    assert(false);  // Should not be used
}
