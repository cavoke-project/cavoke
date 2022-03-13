#ifndef CAVOKE_GAME_SESSION_H
#define CAVOKE_GAME_SESSION_H

#include <drogon/utils/Utilities.h>
#include <boost/bimap.hpp>
#include <map>
#include <random>
#include <string>
#include <utility>
#include "cavoke_base_exception.h"
#include "model/games/game.h"

namespace cavoke::server::model {

/// exception for errors thrown during actions with sessions
struct game_session_error : cavoke_base_exception {
    explicit game_session_error(std::string message);
};

struct GameSession {
    explicit GameSession(GameConfig game_config,
                         std::optional<json> game_settings = {});
    GameSession() =
        default;  // FIXME: required by map in `sessions_storage.cpp`

    void add_user(const std::string &user_id);

    [[nodiscard]] int get_player_id(const std::string &user_id) const;

    [[nodiscard]] std::string get_user_id(int player_id) const;

    [[nodiscard]] bool verify_invite_code(const std::string &invite_code) const;

    /// Serializable representation of session for client
    struct GameSessionInfo {
        std::string session_id;
        std::string game_id;
        std::string invite_code;
    };

    [[nodiscard]] GameSessionInfo get_session_info() const;

    [[nodiscard]] std::vector<int> get_occupied_positions() const;

    [[nodiscard]] const json &get_game_settings() const;

private:
    std::string id;
    GameConfig m_game_config;
    std::string m_invite_code;
    boost::bimap<std::string, int> m_userid_to_playerid{};
    json m_game_settings;

private:
    static std::string generate_invite_code();
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameSession::GameSessionInfo,
                                   session_id,
                                   game_id,
                                   invite_code);
}  // namespace cavoke::server::model

#endif  // CAVOKE_GAME_SESSION_H
