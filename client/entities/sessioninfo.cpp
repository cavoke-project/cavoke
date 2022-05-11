#include "sessioninfo.h"
#include <utility>
SessionInfo::SessionInfo() = default;
SessionInfo::SessionInfo(QString _session_id,
                         QString _game_id,
                         int _status,
                         QString _invite_code,
                         QString _host_id,
                         QVector<Player> _players)
    : session_id(std::move(_session_id)),
      game_id(std::move(_game_id)),
      status(_status),
      invite_code(std::move(_invite_code)),
      host_id(std::move(_host_id)),
      players(std::move(_players)) {
}
void SessionInfo::read(const QJsonObject &json) {
    if (json.contains(SESSION_ID) && json[SESSION_ID].isString()) {
        session_id = json[SESSION_ID].toString();
    }

    if (json.contains(GAME_ID) && json[GAME_ID].isString()) {
        game_id = json[GAME_ID].toString();
    }

    if (json.contains(STATUS) && json[STATUS].isDouble()) {
        status = json[STATUS].toInt();
    }

    if (json.contains(INVITE_CODE) && json[INVITE_CODE].isString()) {
        invite_code = json[INVITE_CODE].toString();
    }

    if (json.contains(HOST_ID) && json[HOST_ID].isString()) {
        host_id = json[HOST_ID].toString();
    }

    if (json.contains(PLAYERS) && json[PLAYERS].isArray()) {
        for (auto obj : json[PLAYERS].toArray()) {
            players.push_back(Player());
            players.back().read(obj.toObject());
        }
    }
}
void SessionInfo::write(QJsonObject &json) const {
    json[SESSION_ID] = session_id;
    json[GAME_ID] = game_id;
    json[STATUS] = status;
    json[INVITE_CODE] = invite_code;
    // FIXME: not implemented, And I don't think it will be necessary
    //    json[PLAYERS] = players;
}
