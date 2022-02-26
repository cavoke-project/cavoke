#include "game_session.h"
cavoke::server::model::game_session_error::game_session_error(
    std::string message)
    : cavoke_base_exception(std::move(message), "cavoke/sessions") {
}
cavoke::server::model::GameSession::GameSession(
    cavoke::server::model::GameConfig game_config)
    : id(drogon::utils::getUuid()),
      m_game_config(std::move(game_config)),
      m_invite_code(generate_invite_code()) {
}
/**
 * Adds given user to session.
 * Throws an exception if user already in this session or too many players in a
 * session
 */
void cavoke::server::model::GameSession::add_user(std::string user_id) {
    // TODO: thread-safety
    if (m_userid_to_playerid.count(user_id) != 0) {
        throw game_session_error("player already in session");
    }
    // get player id for user
    unsigned int pos = m_userid_to_playerid.size();
    if (pos >= m_game_config.players_num) {
        throw game_session_error("maximum number of players reached");
    }
    // add user to session
    m_userid_to_playerid.emplace(std::move(user_id), pos);
}
/**
 * Gets player id for user.
 * Throws an exception if user not in this session
 */
int cavoke::server::model::GameSession::get_player_id(
    const std::string &user_id) const {
    auto it = m_userid_to_playerid.find(user_id);
    if (it == m_userid_to_playerid.end()) {
        throw game_session_error("player not in session");
    }
    return it->second;
}
/// Validates the invite code for this session
bool cavoke::server::model::GameSession::verify_invite_code(
    const std::string &invite_code) const {
    return invite_code == m_invite_code;
}
/// Generates an info object (representation for client)
cavoke::server::model::GameSession::game_session_info
cavoke::server::model::GameSession::get_session_info() const {
    return {id, m_game_config.id, m_invite_code};
}
/// Generates an invite code for session
std::string cavoke::server::model::GameSession::generate_invite_code() {
    // prepare template
    std::string res = "......";
    // random digits
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<char> dist('0', '9');
    // set every character to be a random digit
    for (char &c : res) {
        c = dist(engine);
    }
    return res;
}
