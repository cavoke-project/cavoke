#include "user.h"

User::User() = default;

User::User(QString _display_name, QString _user_id)
    : display_name(std::move(_display_name)), user_id(std::move(_user_id)) {
}

void User::read(const QJsonObject &json) {
    if (json.contains(DISPLAY_NAME) && json[DISPLAY_NAME].isString()) {
        display_name = json[DISPLAY_NAME].toString();
    }
    if (json.contains(USER_ID) && json[USER_ID].isString()) {
        user_id = json[USER_ID].toString();
    }
}

void User::write(QJsonObject &json) {
    assert(false);  // Should not be used
}
