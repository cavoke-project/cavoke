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
    if (json.contains("game_id") && json["game_id"].isString())
        game_id = json["game_id"].toString();

    if (json.contains("session_id") && json["session_id"].isString())
        session_id = json["session_id"].toString();

    if (json.contains("invite_code") && json["invite_code"].isString())
        invite_code = json["invite_code"].toString();
}
void SessionInfo::write(QJsonObject &json) const {
    json["game_id"] = game_id;
    json["session_id"] = session_id;
    json["invite_code"] = invite_code;
}
