#include "sessions_storage.h"
#include <utility>

namespace cavoke::server::model {

/**
 * Creates a session with given game config.
 *
 * Throws `game_session_error` if errors arise
 *
 * @return session info
 */
GameSession::GameSessionInfo SessionsStorage::create_session(
    const GameConfig &game_config,
    const std::string &host_user_id) {
    // create session
    auto session = GameSession(game_config);
    // add host
    session.add_user(host_user_id);
    // generate representation for user
    auto session_info = session.get_session_info();
    // save session
    m_sessions.emplace(session_info.session_id, std::move(session));
    // associate invite code with session
    // TODO: there are only 1e6 invite codes, something has to be done about
    // collisions
    m_invite_codes_to_session_ids[session_info.invite_code] =
        session_info.session_id;
    return session_info;
}

/**
 * Joins a session by invite code
 *
 * Throws `game_session_error` if errors arise
 *
 * @return session info
 */
GameSession::GameSessionInfo SessionsStorage::join_session(
    const std::string &invite_code,
    const std::string &user_id) {
    // find session by invite
    try {
        auto &session =
            m_sessions[m_invite_codes_to_session_ids.at(invite_code)];
        // validate invite code
        if (!session.verify_invite_code(invite_code)) {
            throw game_session_error("invalid invite code");
        }
        // add the user
        session.add_user(user_id);
        // generate representation for client
        return session.get_session_info();
    } catch (const std::out_of_range &) {
        throw game_session_error("invite code does not exist: '" + invite_code +
                                 "'");
    }
}
/**
 * Gets a session for given session id
 *
 * Throws `game_session_error` if no such session
 */
GameSession *SessionsStorage::get_session(const std::string &session_id) {
    try {  // slow?
        return &m_sessions.at(session_id);
    } catch (const std::out_of_range &) {
        throw game_session_error("session does not exist: '" + session_id +
                                 "'");
    }
}

GameLogicManager::ValidationResult SessionsStorage::validate_session(
    const std::string &session_id,
    std::optional<json> game_settings) {
    auto session = get_session(session_id);
    std::string game_id = session->get_session_info().game_id;

    if (!game_settings.has_value()) {
        game_settings = m_games_storage->get_game_by_id(game_id)
                            .value()
                            .config.default_settings;
    }

    return m_game_logic_manager->validate_settings(
        game_id, game_settings.value(), session->get_occupied_positions());
}

void SessionsStorage::start_session(const std::string &session_id,
                                    std::optional<json> game_settings) {
    // TODO: thread-safety
    auto session = get_session(session_id);
    std::string game_id = session->get_session_info().game_id;

    if (!game_settings.has_value()) {
        game_settings = m_games_storage->get_game_by_id(game_id)
                            .value()
                            .config.default_settings;
    }

    auto validation_result = m_game_logic_manager->validate_settings(
        game_id, game_settings.value(), session->get_occupied_positions());

    if (!validation_result.success) {
        throw validation_error(validation_result.message);
    }

    session->start(game_settings.value());
}

SessionsStorage::SessionsStorage(
    std::shared_ptr<GameLogicManager> mGameLogicManager,
    std::shared_ptr<GamesStorage> mGamesStorage)
    : m_game_logic_manager(std::move(mGameLogicManager)),
      m_games_storage(std::move(mGamesStorage)) {
}

}  // namespace cavoke::server::model
