#ifndef CAVOKE_GAME_SESSION_H
#define CAVOKE_GAME_SESSION_H

#include <drogon/HttpAppFramework.h>
#include <drogon/utils/Utilities.h>
#include <boost/bimap.hpp>
#include <map>
#include <random>
#include <shared_mutex>
#include <string>
#include <utility>
#include "cavoke.h"
#include "cavoke_base_exception.h"
#include "model/games/game.h"
#include "sql-models/Players.h"
#include "sql-models/Sessions.h"

namespace cavoke::server::model {

/// exception for errors thrown during actions with sessions
struct game_session_error : cavoke_base_exception {
    explicit game_session_error(std::string message);
};

struct GameSessionAccessObject {
    enum SessionStatus { NOT_STARTED = 0, RUNNING = 1, FINISHED = 2 };
    GameSessionAccessObject() = default;
    explicit GameSessionAccessObject(std::string session_id,
                                     GameConfig game_config)
        : id(std::move(session_id)), m_game_config(std::move(game_config)) {
    }

    void add_user(const std::string &user_id,
                  std::optional<int> player_id = {});

    void start(const json &game_settings);

    void finish();

    [[nodiscard]] int get_player_id(const std::string &user_id) const;

    [[nodiscard]] bool is_player(const std::string &user_id) const;

    [[nodiscard]] std::string get_user_id(int player_id) const;

    [[nodiscard]] bool verify_invite_code(const std::string &invite_code) const;

    /// Serializable representation of participant for client
    struct PlayerInfo {
        std::string user_id;
        int player_id;
    };

    /// Serializable representation of session for client
    struct GameSessionInfo {
        std::string session_id;
        std::string game_id;
        std::string invite_code;
        SessionStatus status;
        std::vector<PlayerInfo> players;
    };

    [[nodiscard]] GameSessionInfo get_session_info() const;

    [[nodiscard]] std::vector<int> get_occupied_positions() const;

    [[nodiscard]] std::vector<PlayerInfo> get_players() const;

    //    [[nodiscard]] std::optional<json> &get_game_settings() const;

    static drogon_model::cavoke_test::Sessions get_snapshot(
        const std::string &session_id);

    static GameSessionInfo make_session_info(
        const drogon_model::cavoke_test::Sessions &session,
        std::vector<PlayerInfo> players);

private:
    std::string id;
    GameConfig m_game_config;

    mutable drogon::orm::Mapper<drogon_model::cavoke_test::Sessions>
        default_mp_sessions{drogon::app().getDbClient()};
    mutable drogon::orm::Mapper<drogon_model::cavoke_test::Players>
        default_mp_players{drogon::app().getDbClient()};

    drogon_model::cavoke_test::Sessions get_snapshot() const;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameSessionAccessObject::PlayerInfo,
                                   user_id,
                                   player_id);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameSessionAccessObject::GameSessionInfo,
                                   session_id,
                                   game_id,
                                   invite_code,
                                   status,
                                   players);
}  // namespace cavoke::server::model

#endif  // CAVOKE_GAME_SESSION_H
