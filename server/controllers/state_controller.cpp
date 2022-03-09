#include "state_controller.h"
#include <utility>

namespace cavoke::server::controllers {

void StateController::send_move(
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
        auto &session = m_participation_storage->get_session(session_id);
        session_info = session.get_session_info();
        player_id = session.get_player_id(user_id.value());
    } catch (const model::game_session_error &) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }

    // TODO: nlohman/json
    std::string body(req->getBody());
    Json::Reader reader;
    Json::Value json_body;
    bool valid_json = reader.parse(body, json_body);
    if (!valid_json || !json_body.isMember("move")) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }

    std::string move = json_body["move"].asString();

    std::optional<model::GameStateStorage::GameState> current_state =
        m_game_state_storage->get_state(session_id);

    if (!current_state.has_value()) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }

    if (current_state->is_terminal) {
        return CALLBACK_STATUS_CODE(k403Forbidden);
    }

    current_state = m_game_logic_manager->send_move(
        session_info.game_id, {player_id, move, current_state->global_state});

    m_game_state_storage->save_state(session_id, current_state.value());

    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::HttpStatusCode::k200OK);
    callback(resp);
}

void StateController::get_state(
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
        player_id = m_participation_storage->get_session(session_id)
                        .get_player_id(user_id.value());
    } catch (const model::game_session_error &) {
        return CALLBACK_STATUS_CODE(k403Forbidden);
    }

    auto state = m_game_state_storage->get_player_state(session_id, player_id);

    if (!state.has_value()) {
        auto resp = drogon::HttpResponse::newNotFoundResponse();
        callback(resp);
        return;
    }

    Json::Value resp_json;
    resp_json["state"] = state.value();
    resp_json["is_terminal"] =
        m_game_state_storage->get_state(session_id)->is_terminal;
    auto resp = drogon::HttpResponse::newHttpJsonResponse(resp_json);
    resp->setStatusCode(drogon::HttpStatusCode::k200OK);
    callback(resp);
}

StateController::StateController(
    std::shared_ptr<model::GamesStorage> mGamesStorage,
    std::shared_ptr<model::GameLogicManager> mGameLogicManager,
    std::shared_ptr<model::GameStateStorage> mGameStateStorage,
    std::shared_ptr<model::SessionsStorage> mParticipationStorage)
    : m_games_storage(std::move(mGamesStorage)),
      m_game_logic_manager(std::move(mGameLogicManager)),
      m_game_state_storage(std::move(mGameStateStorage)),
      m_participation_storage(std::move(mParticipationStorage)) {
}

}  // namespace cavoke::server::controllers
