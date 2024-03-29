#include "state_controller.h"
#include <utility>

namespace cavoke::server::controllers {

using json = nlohmann::json;

void StateController::send_move(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &session_id) {
    // get user id
    auto user_id = AuthFilter::get_user_id(req);

    {
        auto transaction = drogon::app().getDbClient()->newTransaction();
        model::GameSessionAccessObject session_with_transaction;
        try {
            session_with_transaction =
                m_participation_storage->get_sessionAO(session_id, transaction);
        } catch (const model::game_session_error &) {
            transaction->rollback();
            return CALLBACK_STATUS_CODE(k400BadRequest);
        }

        int player_id;
        try {
            player_id = session_with_transaction.get_player_id(user_id);
        } catch (const model::game_session_error &) {
            transaction->rollback();
            return CALLBACK_STATUS_CODE(k403Forbidden);
        }

        auto session_info = session_with_transaction.get_session_info();

        if (session_info.status == model::GameSessionAccessObject::FINISHED) {
            transaction->rollback();
            return CALLBACK_STATUS_CODE(k403Forbidden);
        }
        if (session_info.status ==
            model::GameSessionAccessObject::NOT_STARTED) {
            transaction->rollback();
            return CALLBACK_STATUS_CODE(k404NotFound);
        }

        json json_body;
        try {
            json_body = json::parse(req->getBody());
        } catch (const json::parse_error &) {
            transaction->rollback();
            return CALLBACK_STATUS_CODE(k400BadRequest);
        }
        if (!json_body.contains("move") || !json_body["move"].is_string()) {
            transaction->rollback();
            return CALLBACK_STATUS_CODE(k400BadRequest);
        }

        std::string move = json_body["move"];

        model::GameStateStorage::GameState current_state;
        try {
            current_state =
                m_game_state_storage->get_state(session_id, transaction);
        } catch (const model::game_state_error &) {
            transaction->rollback();
            return CALLBACK_STATUS_CODE(k404NotFound);
        }

        auto next_state = m_game_logic_manager->send_move(
            session_info.game_id,
            {player_id, move, current_state.global_state});

        m_game_state_storage->save_state(session_id, next_state, transaction);

        session_with_transaction.update_status(next_state.is_terminal);
        if (next_state.is_terminal) {
            LOG_INFO << "Session " << session_id
                     << " is being declared finished!";
        }

        transaction->setCommitCallback([next_state, player_id](bool committed) {
            LOG_TRACE << "State set (" << committed << ") state: '"
                      << next_state.global_state << "' player_id=" << player_id;
        });
    }

    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setStatusCode(drogon::HttpStatusCode::k200OK);
    callback(resp);
}

void StateController::get_state(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &session_id) {
    // get user id
    auto user_id = AuthFilter::get_user_id(req);

    auto transaction = drogon::app().getDbClient()->newTransaction();
    model::GameSessionAccessObject session_with_transaction;
    try {
        session_with_transaction =
            m_participation_storage->get_sessionAO(session_id, transaction);
    } catch (const model::game_session_error &) {
        transaction->rollback();
        return CALLBACK_STATUS_CODE(k404NotFound);
    }

    int player_id;
    try {
        player_id = session_with_transaction.get_player_id(user_id);
    } catch (const model::game_session_error &) {
        transaction->rollback();
        return CALLBACK_STATUS_CODE(k403Forbidden);
    }

    auto session_info = session_with_transaction.get_session_info();

    if (session_info.status == model::GameSessionAccessObject::NOT_STARTED) {
        transaction->rollback();
        return CALLBACK_STATUS_CODE(k404NotFound);
    }

    std::string player_state;
    try {
        player_state = m_game_state_storage->get_player_state(
            session_id, player_id, transaction);
    } catch (const model::game_state_error &) {
    }

    json resp_json;
    resp_json["state"] = std::move(player_state);
    auto game_state = m_game_state_storage->get_state(session_id, transaction);
    resp_json["is_terminal"] = game_state.is_terminal;
    if (game_state.is_terminal) {
        std::vector<std::string> winners;
        for (const auto &e : game_state.winners) {
            // cppcheck-suppress useStlAlgorithm
            winners.push_back(session_with_transaction.get_user_id(e));
        }
        resp_json["winners"] = winners;
    }

    transaction->setCommitCallback(
        [player_state = resp_json["state"].get<std::string>(),
         player_id](bool committed) {
            LOG_TRACE << "Got state (" << committed << ") state: '"
                      << player_state << "' player_id=" << player_id;
        });

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
