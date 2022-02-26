#include "sessions_storage.h"

/**
 * Creates a session with given game config.
 *
 * Throws `game_session_error` if errors arise
 *
 * @return session info
 */
cavoke::server::model::GameSession::game_session_info
cavoke::server::model::SessionsStorage::create_session(
    const cavoke::server::model::GameConfig &game_config) {
    // create session
    auto session = GameSession(game_config);
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
cavoke::server::model::GameSession::game_session_info
cavoke::server::model::SessionsStorage::join_session(
    const std::string &invite_code,
    const std::string &user_id) {
    // find session by invite
    auto invite_it = m_invite_codes_to_session_ids.find(invite_code);
    if (invite_it == m_invite_codes_to_session_ids.end()) {
        throw game_session_error("invite code does not exist: '" + invite_code +
                                 "'");
    }
    auto &session = m_sessions[invite_it->second];
    // validate invite code
    if (!session.verify_invite_code(invite_code)) {
        throw game_session_error("invalid invite code");
    }
    // add the user
    session.add_user(user_id);
    // generate representation for client
    return session.get_session_info();
}
/**
 * Gets a session info for given session id
 *
 * Throws `game_session_error` if no such session
 */
cavoke::server::model::GameSession::game_session_info
cavoke::server::model::SessionsStorage::get_session_info(
    const std::string &session_id) {
    auto session_it = m_sessions.find(session_id);
    if (session_it == m_sessions.end()) {
        throw game_session_error("session does not exist: '" + session_id +
                                 "'");
    }
    return session_it->second.get_session_info();
}

/**
 * Gets player id for given session and user
 *
 * Throws `game_session_error` if errors arise
 */
int cavoke::server::model::SessionsStorage::get_player_id(
    const std::string &session_id,
    const std::string &user_id) {
    auto session_it = m_sessions.find(session_id);
    if (session_it == m_sessions.end()) {
        throw game_session_error("session does not exist: '" + session_id +
                                 "'");
    }
    return session_it->second.get_player_id(user_id);
}
