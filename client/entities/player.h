#ifndef CAVOKE_PLAYER_H
#define CAVOKE_PLAYER_H

#include <QtCore/QJsonObject>
#include <QtCore/QString>
#include "user.h"
struct Player {
public:
    Player();
    Player(int _player_id, User _user);

    void read(const QJsonObject &json);
    static void write(QJsonObject &json);

    int player_id{};
    User user;

private:
    static inline const QString PLAYER_ID = "player_id";
    static inline const QString USER = "user";
};

#endif  // CAVOKE_PLAYER_H
