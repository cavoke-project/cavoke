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

    std::optional<int> participant_id =
        m_participation_storage->get_participant_id(session_id,
                                                    user_id.value());

    if (!participant_id.has_value()) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::HttpStatusCode::k403Forbidden);
        callback(resp);
        return;
    }

    std::string body(req->getBody());
    Json::Reader reader;
    Json::Value json_body;
    bool valid_json = reader.parse(body, json_body);
    if (!valid_json || !json_body.isMember("move")) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
        callback(resp);
        return;
    }

    std::string move = json_body["move"].asString();

    std::optional<model::GameStateStorage::GameState> current_state =
        m_game_state_storage->get_state(session_id);

    if (!current_state.has_value()) {
        // new session
        current_state =
            m_game_logic_manager->send_move("tictactoe", {-1, "", ""});
    }

    if (current_state->is_terminal) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::HttpStatusCode::k403Forbidden);
        callback(resp);
        return;
    }

    current_state = m_game_logic_manager->send_move(
        "tictactoe",
        {participant_id.value(), move, current_state->global_state});

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

    std::optional<int> participant_id =
        m_participation_storage->get_participant_id(session_id,
                                                    user_id.value());

    if (!participant_id.has_value()) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::HttpStatusCode::k403Forbidden);
        callback(resp);
        return;
    }

    auto state = m_game_state_storage->get_player_state(session_id,
                                                        participant_id.value());

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

cavoke::server::controllers::StateController::StateController(
    std::shared_ptr<model::GamesStorage> mGamesStorage,
    std::shared_ptr<model::GameLogicManager> mGameLogicManager,
    std::shared_ptr<model::GameStateStorage> mGameStateStorage,
    std::shared_ptr<model::ParticipationStorage> mParticipationStorage)
    : m_games_storage(std::move(mGamesStorage)),
      m_game_logic_manager(std::move(mGameLogicManager)),
      m_game_state_storage(std::move(mGameStateStorage)),
      m_participation_storage(std::move(mParticipationStorage)) {
}
