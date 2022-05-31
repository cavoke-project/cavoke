#ifndef CAVOKE_ROOMINFO_H
#define CAVOKE_ROOMINFO_H

#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QString>
#include <QtCore/QVector>
#include "sessioninfo.h"
#include "user.h"
struct RoomInfo {
public:
    RoomInfo();
    RoomInfo(QString _room_id,
             QString _display_name,
             QString _invite_code,
             QString _session_id,
             QString _host_id,
             QVector<User> _members,
             SessionInfo _session);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    QString room_id;
    QString display_name;
    QString invite_code;
    QString session_id;
    QString host_id;
    QVector<User> members;
    SessionInfo session{};
    bool isSessionAlive = false;
    bool isHost = false;

private:
    static inline const QString ROOM_ID = "room_id";
    static inline const QString DISPLAY_NAME = "display_name";
    static inline const QString INVITE_CODE = "invite_code";
    static inline const QString SESSION_ID = "session_id";
    static inline const QString HOST_ID = "host_id";
    static inline const QString MEMBERS = "members";
    static inline const QString SESSION = "session";
};

#endif  // CAVOKE_ROOMINFO_H
