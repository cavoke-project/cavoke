#ifndef CAVOKE_SESSIONINFO_H
#define CAVOKE_SESSIONINFO_H

#include <QtCore/QJsonObject>
#include <QtCore/QString>
struct SessionInfo {
public:
    SessionInfo();
    SessionInfo(QString _game_id, QString _session_id, QString _invite_code);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    QString game_id;
    QString session_id;
    QString invite_code;

private:
    static inline const QString GAME_ID = "game_id";
    static inline const QString SESSION_ID = "session_id";
    static inline const QString INVITE_CODE = "invite_code";
};

#endif  // CAVOKE_SESSIONINFO_H