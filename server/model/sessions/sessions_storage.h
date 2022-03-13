#ifndef CAVOKE_SERVER_SESSIONS_STORAGE_H
#define CAVOKE_SERVER_SESSIONS_STORAGE_H

#include <map>
#include <string>
#include "model/logic/game_logic_manager.h"
#include "model/sessions/game_session.h"

namespace cavoke::server::model {

class SessionsStorage {
    // TODO: thread-safety

public:
    GameSession::GameSessionInfo create_session(
        const GameConfig &game_config,
        const std::string &host_user_id);

    GameSession::GameSessionInfo join_session(const std::string &invite_code,
                                              const std::string &user_id,
                                              std::optional<int> player_id = {});

    void start_session(const std::string &session_id,
                       std::optional<json> game_settings = {});

    GameLogicManager::ValidationResult validate_session(
        const std::string &session_id,
        std::optional<json> game_settings = {});

    GameSession *get_session(const std::string &session_id);

    GameSession *get_session_by_invite_code(const std::string &session_id);

    SessionsStorage(std::shared_ptr<GameLogicManager> mGameLogicManager,
                    std::shared_ptr<GamesStorage> mGamesStorage,
                    std::shared_ptr<GameStateStorage> mGameStateStorage);

private:
    std::shared_ptr<GameLogicManager> m_game_logic_manager;
    std::shared_ptr<GamesStorage> m_games_storage;
    std::shared_ptr<GameStateStorage> m_game_state_storage;
    std::map<std::string, GameSession> m_sessions{};
    std::map<std::string, std::string> m_invite_codes_to_session_ids{};
};

}  // namespace cavoke::server::model

#endif  // CAVOKE_SERVER_SESSIONS_STORAGE_H
