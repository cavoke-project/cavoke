#include "sessions_controller.h"

namespace cavoke::server::controllers {

using json = nlohmann::json;

void SessionsController::create(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    // get user id
    auto user_id = AuthFilter::get_user_id(req);

    // get game id
    auto game_id = req->getOptionalParameter<std::string>("game_id");
    if (!game_id.has_value()) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }
    // verify game id and acquire game config
    auto game = m_games_storage->get_game_by_id(game_id.value());
    if (!game.has_value()) {
        return callback(newCavokeErrorResponse(
            {"game '" + game_id.value() + "' not found",
             cavoke_base_exception::error_code::NotFound},
            drogon::k404NotFound));
    }

    // create session and acquire session info
    model::GameSessionAccessObject::GameSessionInfo session_info;
    try {
        session_info = m_participation_storage->create_session(
            game.value().config, user_id, "");
    } catch (const model::game_session_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k400BadRequest));
    }
    return callback(newNlohmannJsonResponse(session_info));
}

void SessionsController::join(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &session_id) {
    // get user id
    auto user_id = AuthFilter::get_user_id(req);

    // join session and acquire session info
    model::GameSessionAccessObject::GameSessionInfo session_info;
    try {
        session_info = m_participation_storage->join_session(
            session_id, user_id, req->getOptionalParameter<int>("position"));
    } catch (const model::game_session_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k400BadRequest));
    }

    return callback(newNlohmannJsonResponse(session_info));
}

SessionsController::SessionsController(
    std::shared_ptr<model::GamesStorage> mGamesStorage,
    std::shared_ptr<model::GameLogicManager> mGameLogicManager,
    std::shared_ptr<model::GameStateStorage> mGameStateStorage,
    std::shared_ptr<model::SessionsStorage> mParticipationStorage)
    : m_games_storage(std::move(mGamesStorage)),
      m_game_logic_manager(std::move(mGameLogicManager)),
      m_game_state_storage(std::move(mGameStateStorage)),
      m_participation_storage(std::move(mParticipationStorage)) {
}

void SessionsController::validate(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &session_id) {
    // get user id
    auto user_id = AuthFilter::get_user_id(req);

    model::GameSessionAccessObject session;
    try {
        session = m_participation_storage->get_sessionAO(session_id);
    } catch (const model::game_session_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k404NotFound));
    }

    if (!session.is_player(user_id)) {
        return CALLBACK_STATUS_CODE(k403Forbidden);
    }

    // session does exist
    json result = m_participation_storage->validate_session(session_id, {});
    if (result["success"].get<bool>()) {
        result.erase(result.find("message"));
    }
    return callback(newNlohmannJsonResponse(result));
}

void SessionsController::start(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &session_id) {
    // get user id
    auto user_id = AuthFilter::get_user_id(req);

    model::GameSessionAccessObject session;
    try {
        session = m_participation_storage->get_sessionAO(session_id);
    } catch (const model::game_session_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k404NotFound));
    }

    if (!session.is_player(user_id)) {
        return CALLBACK_STATUS_CODE(k403Forbidden);
    }

    try {
        // TODO: get settings from body
        m_participation_storage->start_session(session_id, {});
    } catch (const model::validation_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k400BadRequest));
    } catch (const model::game_session_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k400BadRequest));
    }

    return CALLBACK_STATUS_CODE(k200OK);
}

void SessionsController::get_info(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &session_id) {
    model::GameSessionAccessObject session;
    try {
        session = m_participation_storage->get_sessionAO(session_id);
    } catch (const model::game_session_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k404NotFound));
    }

    return callback(newNlohmannJsonResponse(session.get_session_info()));
}

void SessionsController::leave(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &session_id) {
    // get user id
    auto user_id = AuthFilter::get_user_id(req);

    model::GameSessionAccessObject session;
    try {
        session = m_participation_storage->get_sessionAO(session_id);
    } catch (const model::game_session_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k404NotFound));
    }

    session.leave_session(user_id);

    return CALLBACK_STATUS_CODE(k200OK);
}

void SessionsController::change_role(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &session_id) {
    // get user id
    auto user_id = AuthFilter::get_user_id(req);

    // get new_role
    auto new_role = req->getOptionalParameter<int>("new_role");
    if (!new_role.has_value()) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }

    model::GameSessionAccessObject session;
    try {
        session = m_participation_storage->get_sessionAO(session_id);
    } catch (const model::game_session_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k404NotFound));
    }

    try {
        session.set_role(user_id, new_role.value());
    } catch (model::game_session_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k400BadRequest));
    }

    return CALLBACK_STATUS_CODE(k200OK);
}

}  // namespace cavoke::server::controllers
