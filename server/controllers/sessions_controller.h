#ifndef CAVOKE_SESSIONS_CONTROLLER_H
#define CAVOKE_SESSIONS_CONTROLLER_H

#include <drogon/HttpController.h>
#include "filters/AuthFilter.h"
#include "model/games/games_storage.h"
#include "model/logic/game_logic_manager.h"
#include "model/sessions/sessions_storage.h"

namespace cavoke::server::controllers {

class SessionsController
    : public drogon::HttpController<SessionsController, false> {
    std::shared_ptr<model::GamesStorage> m_games_storage;
    std::shared_ptr<model::GameLogicManager> m_game_logic_manager;
    std::shared_ptr<model::GameStateStorage> m_game_state_storage;
    std::shared_ptr<model::SessionsStorage> m_participation_storage;

public:
    SessionsController(
        std::shared_ptr<model::GamesStorage> mGamesStorage,
        std::shared_ptr<model::GameLogicManager> mGameLogicManager,
        std::shared_ptr<model::GameStateStorage> mGameStateStorage,
        std::shared_ptr<model::SessionsStorage> mParticipationStorage);

public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(SessionsController::create,
                  "/sessions/create",
                  drogon::Post,
                  "AuthFilter");
    ADD_METHOD_TO(SessionsController::join,
                  "/sessions/join",
                  drogon::Post,
                  "AuthFilter");
    ADD_METHOD_TO(SessionsController::leave,
                  "/sessions/{session_id}/leave",
                  drogon::Post,
                  "AuthFilter");
    ADD_METHOD_TO(SessionsController::change_role,
                  "/sessions/{session_id}/change_role",
                  drogon::Post,
                  "AuthFilter");
    ADD_METHOD_TO(SessionsController::get_info,
                  "/sessions/{session_id}/get_info",
                  drogon::Get,
                  "AuthFilter");
    ADD_METHOD_TO(SessionsController::get_info_by_invite_code,
                  "/sessions/get_info_by_invite_code",
                  drogon::Get,
                  "AuthFilter");
    ADD_METHOD_TO(SessionsController::start,
                  "/sessions/{session_id}/start",
                  drogon::Post,
                  "AuthFilter");
    ADD_METHOD_TO(SessionsController::validate,
                  "/sessions/{session_id}/validate",
                  drogon::Post,
                  "AuthFilter");
    METHOD_LIST_END

protected:
    void create(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);

    void join(const drogon::HttpRequestPtr &req,
              std::function<void(const drogon::HttpResponsePtr &)> &&callback);

    void leave(const drogon::HttpRequestPtr &req,
               std::function<void(const drogon::HttpResponsePtr &)> &&callback,
               const std::string &session_id);

    void change_role(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback,
        const std::string &session_id);

    void validate(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback,
        const std::string &session_id);

    void start(const drogon::HttpRequestPtr &req,
               std::function<void(const drogon::HttpResponsePtr &)> &&callback,
               const std::string &session_id);

    void get_info(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback,
        const std::string &session_id);

    void get_info_by_invite_code(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};

}  // namespace cavoke::server::controllers
#endif  // CAVOKE_SESSIONS_CONTROLLER_H
