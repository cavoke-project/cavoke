#include "player.h"

Player::Player() = default;

Player::Player(int _player_id, QString _user_id)
    : player_id(_player_id), user_id(std::move(_user_id)) {
}

void Player::read(const QJsonObject &json) {
    if (json.contains(USER_ID) && json[USER_ID].isString()) {
        user_id = json[USER_ID].toString();
    }
    if (json.contains(PLAYER_ID) &&
        json[PLAYER_ID].isDouble()) {  // FIXME: wat?
        player_id = json[PLAYER_ID].toInt();
    }
}

void Player::write(QJsonObject &json) const {
    json[USER_ID] = user_id;
    json[PLAYER_ID] = player_id;
}
