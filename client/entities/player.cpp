#include "player.h"

Player::Player() = default;

Player::Player(int _player_id, User _user)
    : player_id(_player_id), user(std::move(_user)) {
}

void Player::read(const QJsonObject &json) {
    if (json.contains(PLAYER_ID) &&
        json[PLAYER_ID].isDouble()) {  // FIXME: wat?
        player_id = json[PLAYER_ID].toInt();
    }
    if (json.contains(USER) && json[USER].isObject()) {
        user = User();
        user.read(json[USER].toObject());
    }
}

void Player::write(QJsonObject &json) {
    assert(false);  // Should not be used
}
