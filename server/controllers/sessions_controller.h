#ifndef CAVOKE_SESSIONS_CONTROLLER_H
#define CAVOKE_SESSIONS_CONTROLLER_H

#include <drogon/HttpController.h>
#include "model/auth/authentication_manager.h"
#include "model/logic/game_logic_manager.h"
#include "model/games/games_storage.h"
#include "model/sessions/sessions_storage.h"

namespace cavoke::server::controllers {

class SessionsController
    : public drogon::HttpController<SessionsController, false> {
    std::shared_ptr<model::GamesStorage> m_games_storage;
    std::shared_ptr<model::GameLogicManager> m_game_logic_manager;
    std::shared_ptr<model::GameStateStorage> m_game_state_storage;
    std::shared_ptr<model::SessionsStorage> m_participation_storage;
    std::shared_ptr<model::AuthenticationManager> m_authentication_manager;

public:
    SessionsController(
        std::shared_ptr<model::GamesStorage> mGamesStorage,
        std::shared_ptr<model::GameLogicManager> mGameLogicManager,
        std::shared_ptr<model::GameStateStorage> mGameStateStorage,
        std::shared_ptr<model::SessionsStorage> mParticipationStorage,
        std::shared_ptr<model::AuthenticationManager> mAuthenticationManager);

public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(SessionsController::create, "/sessions/create", drogon::Post);
    ADD_METHOD_TO(SessionsController::join, "/sessions/join", drogon::Post);
    METHOD_LIST_END

protected:
    void create(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);

    void join(const drogon::HttpRequestPtr &req,
              std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};

}  // namespace cavoke::server::controllers
#endif  // CAVOKE_SESSIONS_CONTROLLER_H
