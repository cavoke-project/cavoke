#include "state_controller.h"
#include <utility>

namespace cavoke::server::controllers {

using json = nlohmann::json;

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

    std::shared_ptr<model::GameSession> session;
    try {
        session = m_participation_storage->get_session(session_id);
    } catch (const model::game_session_error &) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }

    int player_id;
    try {
        player_id = session->get_player_id(user_id.value());
    } catch (const model::game_session_error &) {
        return CALLBACK_STATUS_CODE(k403Forbidden);
    }

    auto session_info = session->get_session_info();

    if (session_info.status == model::GameSession::FINISHED) {
        return CALLBACK_STATUS_CODE(k403Forbidden);
    }
    if (session_info.status == model::GameSession::NOT_STARTED) {
        return CALLBACK_STATUS_CODE(k404NotFound);
    }

    json json_body;
    try {
        json_body = json::parse(req->getBody());
    } catch (const json::parse_error &) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }
    if (!json_body.contains("move")) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }

    std::string move = json_body["move"];

    model::GameStateStorage::GameState current_state;
    try {
        current_state = m_game_state_storage->get_state(session_id);
    } catch (const model::game_state_error &) {
        return CALLBACK_STATUS_CODE(k404NotFound);
    }

    auto next_state = m_game_logic_manager->send_move(
        session_info.game_id, {player_id, move, current_state.global_state});

    m_game_state_storage->save_state(session_id, next_state);

    if (next_state.is_terminal) {
        session->finish();
    }

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

    std::shared_ptr<model::GameSession> session;
    try {
        session = m_participation_storage->get_session(session_id);
    } catch (const model::game_session_error &) {
        return CALLBACK_STATUS_CODE(k404NotFound);
    }

    int player_id;
    try {
        player_id = session->get_player_id(user_id.value());
    } catch (const model::game_session_error &) {
        return CALLBACK_STATUS_CODE(k403Forbidden);
    }

    auto session_info = session->get_session_info();

    if (session_info.status == model::GameSession::NOT_STARTED) {
        return CALLBACK_STATUS_CODE(k404NotFound);
    }

    std::string player_state;
    try {
        player_state =
            m_game_state_storage->get_player_state(session_id, player_id);
    } catch (const model::game_state_error &) {
    }

    json resp_json;
    resp_json["state"] = std::move(player_state);
    auto game_state = m_game_state_storage->get_state(session_id);
    resp_json["is_terminal"] = game_state.is_terminal;
    if (game_state.is_terminal) {
        std::vector<std::string> winners;
        for (const auto &e : game_state.winners) {
            // cppcheck-suppress useStlAlgorithm
            winners.push_back(session->get_user_id(e));
        }
        resp_json["winners"] = winners;
    }

    auto resp = newNlohmannJsonResponse(resp_json);
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
