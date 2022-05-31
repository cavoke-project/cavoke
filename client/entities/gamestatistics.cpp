#include "gamestatistics.h"

GameStatistics::GameStatistics() = default;

GameStatistics::GameStatistics(int _average_duration_sec,
                               int _average_players_count,
                               int _total_time_played_sec,
                               int _total_games_played)
    : average_duration_sec(_average_duration_sec),
      average_players_count(_average_players_count),
      total_time_played_sec(_total_time_played_sec),
      total_games_played(_total_games_played) {
}

void GameStatistics::read(const QJsonObject &json) {
    if (json.contains(AVERAGE_DURATION_SEC) &&
        json[AVERAGE_DURATION_SEC].isDouble()) {
        average_duration_sec = json[AVERAGE_DURATION_SEC].toInt();
    }
    if (json.contains(AVERAGE_PLAYERS_COUNT) &&
        json[AVERAGE_PLAYERS_COUNT].isDouble()) {
        average_players_count = json[AVERAGE_PLAYERS_COUNT].toInt();
    }
    if (json.contains(TOTAL_TIME_PLAYED_SEC) &&
        json[TOTAL_TIME_PLAYED_SEC].isDouble()) {
        total_time_played_sec = json[TOTAL_TIME_PLAYED_SEC].toInt();
    }
    if (json.contains(TOTAL_GAMES_PLAYED) &&
        json[TOTAL_GAMES_PLAYED].isDouble()) {
        total_games_played = json[TOTAL_GAMES_PLAYED].toInt();
    }
}

void GameStatistics::write(QJsonObject &json) {
    assert(false);  // Should not be used
}
