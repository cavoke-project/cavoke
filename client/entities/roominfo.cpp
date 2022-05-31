#include "roominfo.h"
#include <utility>
RoomInfo::RoomInfo() = default;
RoomInfo::RoomInfo(QString _room_id,
                   QString _display_name,
                   QString _invite_code,
                   QString _session_id,
                   QString _host_id,
                   QVector<User> _members)
    : room_id(std::move(_room_id)),
      display_name(std::move(_display_name)),
      invite_code(std::move(_invite_code)),
      session_id(std::move(_session_id)),
      host_id(std::move(_host_id)),
      members(std::move(_members)) {
}
void RoomInfo::read(const QJsonObject &json) {
    if (json.contains(ROOM_ID) && json[ROOM_ID].isString()) {
        room_id = json[ROOM_ID].toString();
    }

    if (json.contains(DISPLAY_NAME) && json[DISPLAY_NAME].isString()) {
        display_name = json[DISPLAY_NAME].toString();
    }

    if (json.contains(INVITE_CODE) && json[INVITE_CODE].isString()) {
        invite_code = json[INVITE_CODE].toString();
    }

    if (json.contains(SESSION_ID) && json[SESSION_ID].isString()) {
        session_id = json[SESSION_ID].toString();
    }

    if (json.contains(HOST_ID) && json[HOST_ID].isString()) {
        host_id = json[HOST_ID].toString();
    }

    if (json.contains(MEMBERS) && json[MEMBERS].isArray()) {
        for (auto obj : json[MEMBERS].toArray()) {
            members.push_back(User());
            members.back().read(obj.toObject());
        }
    }
}
void RoomInfo::write(QJsonObject &json) const {
    assert(false);  // Should not be used
}
