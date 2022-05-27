#ifndef CAVOKE_SESSIONINFO_H
#define CAVOKE_SESSIONINFO_H

#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QString>
#include <QtCore/QVector>
#include "player.h"
struct SessionInfo {
public:
    SessionInfo();
    SessionInfo(QString _session_id,
                QString _game_id,
                int _status,
                QString _host_id,
                QVector<Player> _players);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    QString session_id;
    QString game_id;
    int status = 0;  // 0 -- NOT STARTED, 1 -- RUNNING, 2 -- FINISHED
    QString host_id;
    QVector<Player> players;
    bool isHost = false;

private:
    static inline const QString SESSION_ID = "session_id";
    static inline const QString GAME_ID = "game_id";
    static inline const QString STATUS = "status";
    static inline const QString HOST_ID = "host_id";
    static inline const QString PLAYERS = "players";
};

#endif  // CAVOKE_SESSIONINFO_H
