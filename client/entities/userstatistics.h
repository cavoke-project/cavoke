#ifndef CAVOKE_USERSTATISTICS_H
#define CAVOKE_USERSTATISTICS_H

#include <QtCore/QJsonObject>
#include <QtCore/QString>
struct UserStatistics {
public:
    UserStatistics();
    UserStatistics(int _total_time_played_sec, int _total_games_played);

    void read(const QJsonObject &json);
    static void write(QJsonObject &json);

    int total_time_played_sec = 0;
    int total_games_played = 0;

private:
    static inline const QString TOTAL_TIME_PLAYED_SEC = "total_time_played_sec";
    static inline const QString TOTAL_GAMES_PLAYED = "total_games_played";
};

#endif  // CAVOKE_USERSTATISTICS_H
