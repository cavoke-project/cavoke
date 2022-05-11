#include "sessions_controller.h"

namespace cavoke::server::controllers {

using json = nlohmann::json;

void SessionsController::create(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    // get user id
    auto user_id = req->getOptionalParameter<std::string>("user_id");
    if (!user_id.has_value()) {
        return CALLBACK_STATUS_CODE(k401Unauthorized);
    }
    // verify auth
    if (!m_authentication_manager->verify_authentication(user_id.value())) {
        return CALLBACK_STATUS_CODE(k401Unauthorized);
    }

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
            game.value().config, user_id.value());
    } catch (const model::game_session_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k400BadRequest));
    }
    return callback(newNlohmannJsonResponse(session_info));
}

void SessionsController::join(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    // get user id
    auto user_id = req->getOptionalParameter<std::string>("user_id");
    if (!user_id.has_value()) {
        return CALLBACK_STATUS_CODE(k401Unauthorized);
    }
    // verify auth
    if (!m_authentication_manager->verify_authentication(user_id.value())) {
        return CALLBACK_STATUS_CODE(k401Unauthorized);
    }

    // get invite code
    auto invite_code = req->getOptionalParameter<std::string>("invite_code");
    if (!invite_code.has_value()) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }

    // join session and acquire session info
    model::GameSessionAccessObject::GameSessionInfo session_info;
    try {
        session_info = m_participation_storage->join_session(
            invite_code.value(), user_id.value(),
            req->getOptionalParameter<int>("position"));
    } catch (const model::game_session_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k400BadRequest));
    }

    return callback(newNlohmannJsonResponse(session_info));
}

SessionsController::SessionsController(
    std::shared_ptr<model::GamesStorage> mGamesStorage,
    std::shared_ptr<model::GameLogicManager> mGameLogicManager,
    std::shared_ptr<model::GameStateStorage> mGameStateStorage,
    std::shared_ptr<model::SessionsStorage> mParticipationStorage,
    std::shared_ptr<model::AuthenticationManager> mAuthenticationManager)
    : m_games_storage(std::move(mGamesStorage)),
      m_game_logic_manager(std::move(mGameLogicManager)),
      m_game_state_storage(std::move(mGameStateStorage)),
      m_participation_storage(std::move(mParticipationStorage)),
      m_authentication_manager(std::move(mAuthenticationManager)) {
}

void SessionsController::validate(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &session_id) {
    // get user id
    auto user_id = req->getOptionalParameter<std::string>("user_id");
    if (!user_id.has_value()) {
        return CALLBACK_STATUS_CODE(k401Unauthorized);
    }
    // verify auth
    if (!m_authentication_manager->verify_authentication(user_id.value())) {
        return CALLBACK_STATUS_CODE(k401Unauthorized);
    }

    model::GameSessionAccessObject session;
    try {
        session = m_participation_storage->get_sessionAO(session_id);
    } catch (const model::game_session_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k404NotFound));
    }

    if (!session.is_player(user_id.value())) {
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
    auto user_id = req->getOptionalParameter<std::string>("user_id");
    if (!user_id.has_value()) {
        return CALLBACK_STATUS_CODE(k401Unauthorized);
    }
    // verify auth
    if (!m_authentication_manager->verify_authentication(user_id.value())) {
        return CALLBACK_STATUS_CODE(k401Unauthorized);
    }

    model::GameSessionAccessObject session;
    try {
        session = m_participation_storage->get_sessionAO(session_id);
    } catch (const model::game_session_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k404NotFound));
    }

    if (!session.is_player(user_id.value())) {
        return CALLBACK_STATUS_CODE(k403Forbidden);
    }

    // verify whether the request was from host
    if (!session.is_host(user_id.value())) {
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
    // get user id
    auto user_id = req->getOptionalParameter<std::string>("user_id");
    if (!user_id.has_value()) {
        return CALLBACK_STATUS_CODE(k401Unauthorized);
    }
    // verify auth
    if (!m_authentication_manager->verify_authentication(user_id.value())) {
        return CALLBACK_STATUS_CODE(k401Unauthorized);
    }

    model::GameSessionAccessObject session;
    try {
        session = m_participation_storage->get_sessionAO(session_id);
    } catch (const model::game_session_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k404NotFound));
    }

    return callback(newNlohmannJsonResponse(session.get_session_info()));
}

void SessionsController::get_info_by_invite_code(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    // get invite code
    auto invite_code = req->getOptionalParameter<std::string>("invite_code");
    if (!invite_code.has_value()) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }
    // get user id
    auto user_id = req->getOptionalParameter<std::string>("user_id");
    if (!user_id.has_value()) {
        return CALLBACK_STATUS_CODE(k401Unauthorized);
    }
    // verify auth
    if (!m_authentication_manager->verify_authentication(user_id.value())) {
        return CALLBACK_STATUS_CODE(k401Unauthorized);
    }

    model::GameSessionAccessObject session;
    try {
        session = m_participation_storage->get_sessionAO_by_invite(
            invite_code.value());
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
    auto user_id = req->getOptionalParameter<std::string>("user_id");
    if (!user_id.has_value()) {
        return CALLBACK_STATUS_CODE(k401Unauthorized);
    }
    // verify auth
    if (!m_authentication_manager->verify_authentication(user_id.value())) {
        return CALLBACK_STATUS_CODE(k401Unauthorized);
    }

    drogon::app().getDbClient()->execSqlSync(
        "select transfer_to_available($1::uuid, $2::uuid);", session_id,
        user_id.value());  // what could possibly go wrong?

    return CALLBACK_STATUS_CODE(k200OK);
}

void SessionsController::change_role(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &session_id) {
    // get user id
    auto user_id = req->getOptionalParameter<std::string>("user_id");
    if (!user_id.has_value()) {
        return CALLBACK_STATUS_CODE(k401Unauthorized);
    }

    // get new_role
    auto new_role = req->getOptionalParameter<int>("new_role");
    if (!new_role.has_value()) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }

    // verify auth
    if (!m_authentication_manager->verify_authentication(user_id.value())) {
        return CALLBACK_STATUS_CODE(k401Unauthorized);
    }

    model::GameSessionAccessObject session;
    try {
        session = m_participation_storage->get_sessionAO(session_id);
    } catch (const model::game_session_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k404NotFound));
    }

    try {
        session.set_role(user_id.value(), new_role.value());
    } catch (model::game_session_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k400BadRequest));
    }

    return CALLBACK_STATUS_CODE(k200OK);
}

}  // namespace cavoke::server::controllers
