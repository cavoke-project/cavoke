#ifndef CAVOKE_GAME_SESSION_H
#define CAVOKE_GAME_SESSION_H

#include <drogon/utils/Utilities.h>
#include <map>
#include <random>
#include <string>
#include <utility>
#include "cavoke_base_exception.h"
#include "model/game.h"

namespace cavoke::server::model {

/// exception for errors thrown during actions with sessions
struct game_session_error : cavoke_base_exception {
    explicit game_session_error(std::string message);
};

struct game_session {
    explicit game_session(GameConfig game_config);
    game_session() =
        default;  // FIXME: required by map in `sessions_storage.cpp`

    void add_user(std::string user_id);

    [[nodiscard]] int get_player_id(const std::string &user_id) const;

    [[nodiscard]] bool verify_invite_code(const std::string &invite_code) const;

    /// Serializable representation of session for client
    struct game_session_info {
        std::string session_id;
        std::string game_id;
        std::string invite_code;
    };

    [[nodiscard]] game_session_info get_session_info() const;

private:
    std::string id;
    GameConfig m_game_config;
    std::string m_invite_code;
    std::map<std::string, int> m_userid_to_playerid{};

    static std::string generate_invite_code();
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(game_session::game_session_info,
                                   session_id,
                                   game_id,
                                   invite_code);
}  // namespace cavoke::server::model

#endif  // CAVOKE_GAME_SESSION_H
