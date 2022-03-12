#include "sessions_storage.h"
#include <utility>

/**
 * Creates a session with given game config.
 *
 * Throws `game_session_error` if errors arise
 *
 * @return session info
 */
cavoke::server::model::GameSession::GameSessionInfo
cavoke::server::model::SessionsStorage::create_session(
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
cavoke::server::model::GameSession::GameSessionInfo
cavoke::server::model::SessionsStorage::join_session(
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
cavoke::server::model::GameSession *
cavoke::server::model::SessionsStorage::get_session(
    const std::string &session_id) {
    try {  // slow?
        return &m_sessions.at(session_id);
    } catch (const std::out_of_range &) {
        throw game_session_error("session does not exist: '" + session_id +
                                 "'");
    }
}
