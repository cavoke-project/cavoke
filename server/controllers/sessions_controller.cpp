#include "sessions_controller.h"

void cavoke::server::controllers::SessionsController::create(
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
    model::GameSession::GameSessionInfo session_info;
    // TODO: do we want `try-catch` or `optional`?
    try {
        session_info = m_participation_storage->create_session(
            game.value().config, user_id.value());
    } catch (const model::game_session_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k400BadRequest));
    }

    // configurate initial state for session

    // TODO: validate settings
    // TODO: get actual connected players
    m_game_state_storage->save_state(
        session_info.session_id,
        m_game_logic_manager->init_state(
            game_id.value(), game.value().config.default_settings, {0, 1}));

    return callback(newNlohmannJsonResponse(session_info));
}
void cavoke::server::controllers::SessionsController::join(
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
    model::GameSession::GameSessionInfo session_info;
    // TODO: do we want `try-catch` or `optional`?
    try {
        session_info = m_participation_storage->join_session(
            invite_code.value(), user_id.value());
    } catch (const model::game_session_error &err) {
        return callback(newCavokeErrorResponse(err, drogon::k400BadRequest));
    }

    return callback(newNlohmannJsonResponse(session_info));
}
cavoke::server::controllers::SessionsController::SessionsController(
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
