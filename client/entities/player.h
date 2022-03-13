#ifndef CAVOKE_PLAYER_H
#define CAVOKE_PLAYER_H

#include <QtCore/QJsonObject>
#include <QtCore/QString>
struct Player {
public:
    Player();
    Player(int _player_id, QString _user_id);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    int player_id;
    QString user_id;

private:
    static inline const QString PLAYER_ID = "player_id";
    static inline const QString USER_ID = "user_id";
};

#endif  // CAVOKE_PLAYER_H
