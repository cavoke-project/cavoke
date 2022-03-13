#include "sessioninfo.h"
#include <utility>
SessionInfo::SessionInfo() = default;
SessionInfo::SessionInfo(QString _game_id,
                         QString _session_id,
                         QString _invite_code)
    : game_id(std::move(_game_id)),
      session_id(std::move(_session_id)),
      invite_code(std::move(_invite_code)) {
}
void SessionInfo::read(const QJsonObject &json) {
    if (json.contains(GAME_ID) && json[GAME_ID].isString()) {
        game_id = json[GAME_ID].toString();
    }

    if (json.contains(SESSION_ID) && json[SESSION_ID].isString()) {
        session_id = json[SESSION_ID].toString();
    }

    if (json.contains(INVITE_CODE) && json[INVITE_CODE].isString()) {
        invite_code = json[INVITE_CODE].toString();
    }
}
void SessionInfo::write(QJsonObject &json) const {
    json[GAME_ID] = game_id;
    json[SESSION_ID] = session_id;
    json[INVITE_CODE] = invite_code;
}
