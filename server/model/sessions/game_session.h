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
#include "sql-models/Statuses.h"

namespace cavoke::server::model {

/// exception for errors thrown during actions with sessions
struct game_session_error : cavoke_base_exception {
    explicit game_session_error(std::string message);
};

/// Access object for a game session.
/// The fields of sessions may be spontaneously changed in another instance
/// of the server. Therefore this class only provides methods to fetch DB data.
/// Drogon ORM struct serves as the data holder.
struct GameSessionAccessObject {
    enum SessionStatus { NOT_STARTED = 0, RUNNING = 1, FINISHED = 2 };
    GameSessionAccessObject() = default;
    explicit GameSessionAccessObject(std::string session_id,
                                     GameConfig game_config)
        : id(std::move(session_id)), m_game_config(std::move(game_config)) {
    }

    /// Adds given user to the session into given or minimal available position
    void add_user(const std::string &user_id,
                  std::optional<int> player_id = {});

    /// Removes given user
    void remove_user(const std::string &user_id);

    /// Changes game role for given user
    void set_role(const std::string &user_id, int new_role);

    /// Delete current session
    void delete_session();

    /// Leave current session (and transfer host/delete session)
    void leave_session(const std::string &user_id);

    /// Marks session as starts
    void start(const json &game_settings);

    /// Marks sessions as finished
    void finish();

    [[nodiscard]] int get_player_id(const std::string &user_id) const;

    [[nodiscard]] std::string get_host() const;

    [[nodiscard]] bool is_player(const std::string &user_id) const;

    [[nodiscard]] bool is_host(const std::string &user_id) const;

    [[nodiscard]] std::string get_user_id(int player_id) const;

    // TODO: useless?
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
        std::string host_id;
        SessionStatus status;
        std::vector<PlayerInfo> players;
    };

    [[nodiscard]] GameSessionInfo get_session_info() const;

    [[nodiscard]] std::vector<int> get_occupied_positions() const;

    [[nodiscard]] std::vector<PlayerInfo> get_players() const;

    //    [[nodiscard]] std::optional<json> &get_game_settings() const; //
    //    unused, was non-trivial to implement.

    /// Fetches current session information from the DB
    static drogon_model::cavoke_orm::Sessions get_snapshot(
        const std::string &session_id);

    /// Builds a session info from a session
    static GameSessionInfo make_session_info(
        const drogon_model::cavoke_orm::Sessions &session,
        const drogon_model::cavoke_orm::Statuses &status,
        std::vector<PlayerInfo> players);

private:
    std::string id;
    GameConfig m_game_config;

    mutable drogon::orm::Mapper<drogon_model::cavoke_orm::Sessions>
        default_mp_sessions{drogon::app().getDbClient()};
    mutable drogon::orm::Mapper<drogon_model::cavoke_orm::Statuses>
        default_mp_statuses{drogon::app().getDbClient()};
    mutable drogon::orm::Mapper<drogon_model::cavoke_orm::Players>
        default_mp_players{drogon::app().getDbClient()};

    drogon_model::cavoke_orm::Sessions get_snapshot() const;
    drogon_model::cavoke_orm::Statuses get_actual_status() const;
    void set_status(SessionStatus status);
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameSessionAccessObject::PlayerInfo,
                                   user_id,
                                   player_id);

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameSessionAccessObject::GameSessionInfo,
                                   session_id,
                                   game_id,
                                   invite_code,
                                   host_id,
                                   status,
                                   players);
}  // namespace cavoke::server::model

#endif  // CAVOKE_GAME_SESSION_H
