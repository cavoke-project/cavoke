#ifndef CAVOKE_SERVER_SESSIONS_STORAGE_H
#define CAVOKE_SERVER_SESSIONS_STORAGE_H

#include <map>
#include <string>
#include "game_session.h"
#include "model/logic/game_logic_manager.h"

namespace cavoke::server::model {

class SessionsStorage {
public:
    SessionsStorage(std::shared_ptr<GameLogicManager> mGameLogicManager,
                    std::shared_ptr<GamesStorage> mGamesStorage,
                    std::shared_ptr<GameStateStorage> mGameStateStorage);

    GameSessionAccessObject::GameSessionInfo create_session(
        const GameConfig &game_config,
        const std::string &host_user_id);

    void start_session(const std::string &session_id,
                       std::optional<json> game_settings);

    GameSessionAccessObject::GameSessionInfo join_session(
        const std::string &invite_code,
        const std::string &user_id,
        std::optional<int> player_id = {});

    cavoke::ValidationResult validate_session(
        const std::string &session_id,
        std::optional<json> game_settings);

    GameSessionAccessObject get_sessionAO(const std::string &session_id);
    GameSessionAccessObject get_sessionAO_by_invite(
        const std::string &invite_code);

    std::shared_ptr<GameLogicManager> m_game_logic_manager;
    std::shared_ptr<GamesStorage> m_games_storage;
    std::shared_ptr<GameStateStorage> m_game_state_storage;

private:
    static std::string generate_invite_code();
};

}  // namespace cavoke::server::model

#endif  // CAVOKE_SERVER_SESSIONS_STORAGE_H
