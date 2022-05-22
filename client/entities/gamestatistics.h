#ifndef CAVOKE_GAMESTATISTICS_H
#define CAVOKE_GAMESTATISTICS_H

#include <QtCore/QJsonObject>
#include <QtCore/QString>
struct GameStatistics {
public:
    GameStatistics();
    GameStatistics(int _average_duration_sec,
                   int _average_players_count,
                   int _total_time_played_sec,
                   int _total_games_played);

    void read(const QJsonObject &json);
    static void write(QJsonObject &json);

    int average_duration_sec = 0;
    int average_players_count = 0;
    int total_time_played_sec = 0;
    int total_games_played = 0;

private:
    static inline const QString AVERAGE_DURATION_SEC = "average_duration_sec";
    static inline const QString AVERAGE_PLAYERS_COUNT = "average_players_count";
    static inline const QString TOTAL_TIME_PLAYED_SEC = "total_time_played_sec";
    static inline const QString TOTAL_GAMES_PLAYED = "total_games_played";
};

#endif  // CAVOKE_USERSTATISTICS_H
