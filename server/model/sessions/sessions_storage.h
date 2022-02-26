#ifndef CAVOKE_SERVER_SESSIONS_STORAGE_H
#define CAVOKE_SERVER_SESSIONS_STORAGE_H

#include <map>
#include <string>
#include "model/sessions/game_session.h"

namespace cavoke::server::model {

class SessionsStorage {
    // TODO: thread-safety

public:
    game_session::game_session_info create_session(
        const GameConfig &game_config);

    game_session::game_session_info join_session(const std::string &invite_code,
                                                 const std::string &user_id);

    game_session::game_session_info get_session_info(
        const std::string &session_id);

    int get_player_id(const std::string &session_id,
                      const std::string &user_id);

private:
    std::map<std::string, game_session> m_sessions{};
    std::map<std::string, std::string> m_invite_codes_to_session_ids{};
};

}  // namespace cavoke::server::model

#endif  // CAVOKE_SERVER_SESSIONS_STORAGE_H
