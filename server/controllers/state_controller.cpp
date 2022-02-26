#include "state_controller.h"
#include <utility>

void cavoke::server::controllers::StateController::send_move(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &session_id) {
    std::optional<std::string> user_id =
        req->getOptionalParameter<std::string>("user_id");

    if (!user_id.has_value()) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
        callback(resp);
        return;
    }

    // TODO: do we want `try-catch` or `optional`?
    model::GameSession::GameSessionInfo session_info;
    int player_id;
    try {
        // FIXME: two requests... maybe do it cleaner?
        session_info = m_participation_storage->get_session_info(session_id);
        player_id =
            m_participation_storage->get_player_id(session_id, user_id.value());
    } catch (const model::game_session_error &) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }

    std::optional<model::GameStateStorage::GameState> current_state =
        m_game_state_storage->get_state(session_id);

    current_state = m_game_logic_manager->send_move(
        session_info.game_id,
        {player_id, std::string(req->getBody()), current_state->global_state});

    m_game_state_storage->save_state(session_id, current_state.value());

    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::HttpStatusCode::k200OK);
    callback(resp);
}

void cavoke::server::controllers::StateController::get_update(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &session_id) {
    std::optional<std::string> user_id =
        req->getOptionalParameter<std::string>("user_id");

    if (!user_id.has_value()) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
        callback(resp);
        return;
    }

    // TODO: do we want `try-catch` or `optional`?
    int player_id;
    try {
        player_id =
            m_participation_storage->get_player_id(session_id, user_id.value());
    } catch (const model::game_session_error &) {
        return CALLBACK_STATUS_CODE(k403Forbidden);
    }

    auto state = m_game_state_storage->get_player_state(session_id, player_id);

    if (!state.has_value()) {
        auto resp = drogon::HttpResponse::newNotFoundResponse();
        callback(resp);
        return;
    }

    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::HttpStatusCode::k200OK);
    resp->setBody(state.value());
    callback(resp);
}
cavoke::server::controllers::StateController::StateController(
    std::shared_ptr<model::GamesStorage> mGamesStorage,
    std::shared_ptr<model::GameLogicManager> mGameLogicManager,
    std::shared_ptr<model::GameStateStorage> mGameStateStorage,
    std::shared_ptr<model::SessionsStorage> mParticipationStorage)
    : m_games_storage(std::move(mGamesStorage)),
      m_game_logic_manager(std::move(mGameLogicManager)),
      m_game_state_storage(std::move(mGameStateStorage)),
      m_participation_storage(std::move(mParticipationStorage)) {
}
