#ifndef CAVOKE_STATE_CONTROLLER_H
#define CAVOKE_STATE_CONTROLLER_H

#include <drogon/HttpController.h>
#include "../model/game_logic_manager.h"
#include "../model/games_storage.h"
#include "../model/participation_storage.h"

namespace cavoke::server::controllers {

class StateController : public drogon::HttpController<StateController, false> {
    std::shared_ptr<model::GamesStorage> m_games_storage;
    std::shared_ptr<model::GameLogicManager> m_game_logic_manager;
    std::shared_ptr<model::GameStateStorage> m_game_state_storage;
    std::shared_ptr<model::ParticipationStorage> m_participation_storage;

public:
    StateController(
        std::shared_ptr<model::GamesStorage> mGamesStorage,
        std::shared_ptr<model::GameLogicManager> mGameLogicManager,
        std::shared_ptr<model::GameStateStorage> mGameStateStorage,
        std::shared_ptr<model::ParticipationStorage> mParticipationStorage);

public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(StateController::send_move,
                  "/play/{session_id}/send_move",
                  drogon::Post);
    ADD_METHOD_TO(StateController::get_update,
                  "/play/{session_id}/get_update",
                  drogon::Get);
    METHOD_LIST_END

protected:
    void send_move(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback,
        const std::string &session_id);

    void get_update(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback,
        const std::string &session_id);
};

}  // namespace cavoke::server::controllers

#endif  // CAVOKE_STATE_CONTROLLER_H
