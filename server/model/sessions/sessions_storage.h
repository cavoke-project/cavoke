#ifndef CAVOKE_SERVER_SESSIONS_STORAGE_H
#define CAVOKE_SERVER_SESSIONS_STORAGE_H

#include <map>
#include <string>
#include "game_session.h"
#include "model/logic/game_logic_manager.h"

namespace cavoke::server::model {

class SessionsStorage {
    std::shared_ptr<GameLogicManager> m_game_logic_manager;
    std::shared_ptr<GamesStorage> m_games_storage;
    std::shared_ptr<GameStateStorage> m_game_state_storage;

public:
    SessionsStorage(std::shared_ptr<GameLogicManager> mGameLogicManager,
                    std::shared_ptr<GamesStorage> mGamesStorage,
                    std::shared_ptr<GameStateStorage> mGameStateStorage);

    /// Creates session for given user with given game_config
    GameSessionAccessObject::GameSessionInfo create_session(
        const GameConfig &game_config,
        const std::string &host_user_id,
        const std::string &room_id = "");

    /// Starts session with given settings
    void start_session(const std::string &session_id,
                       std::optional<json> game_settings);

    /// Tries to connect given user into a session
    GameSessionAccessObject::GameSessionInfo join_session(
        const std::string &invite_code,
        const std::string &user_id,
        std::optional<int> player_id = {});

    /// Validates the settings for given session
    cavoke::ValidationResult validate_session(
        const std::string &session_id,
        std::optional<json> game_settings);

    /// Gets an access object for given session
    GameSessionAccessObject get_sessionAO(const std::string &session_id, drogon::orm::DbClientPtr dbClient = drogon::app().getDbClient());

    friend class GameSessionAccessObject;
};

}  // namespace cavoke::server::model

#endif  // CAVOKE_SERVER_SESSIONS_STORAGE_H
