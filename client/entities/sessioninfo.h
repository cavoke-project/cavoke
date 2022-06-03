#ifndef CAVOKE_SESSIONINFO_H
#define CAVOKE_SESSIONINFO_H

#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QString>
#include <QtCore/QVector>
#include "player.h"
struct SessionInfo {
    enum class Status { NOT_STARTED, RUNNING, FINISHED };

public:
    SessionInfo();
    SessionInfo(QString _session_id,
                QString _game_id,
                SessionInfo::Status _status,
                QString _host_id,
                QVector<Player> _players);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    QString session_id;
    QString game_id;
    SessionInfo::Status status = Status::FINISHED;
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
