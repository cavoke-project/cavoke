#ifndef CAVOKE_USERGAMESTATISTICS_H
#define CAVOKE_USERGAMESTATISTICS_H

#include <QJsonObject>
#include <QString>
struct UserGameStatistics {
public:
    UserGameStatistics();
    UserGameStatistics(QString _game_id,
                       int _time_played_sec,
                       int _games_played,
                       double _win_rate);

    void read(const QJsonObject &json);
    static void write(QJsonObject &json);

    QString game_id;
    int time_played_sec = 0;
    int games_played = 0;
    double win_rate = 0;

private:
    static inline const QString GAME_ID = "game_id";
    static inline const QString TIME_PLAYED_SEC = "time_played_sec";
    static inline const QString GAMES_PLAYED = "games_played";
    static inline const QString WIN_RATE = "win_rate";
};

#endif  // CAVOKE_USERGAMESTATISTICS_H
