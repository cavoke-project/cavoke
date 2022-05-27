#include "sessioninfo.h"
#include <utility>
SessionInfo::SessionInfo() = default;
SessionInfo::SessionInfo(QString _session_id,
                         QString _game_id,
                         int _status,
                         QString _host_id,
                         QVector<Player> _players)
    : session_id(std::move(_session_id)),
      game_id(std::move(_game_id)),
      status(_status),
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
    assert(false); // Should not be used
}
