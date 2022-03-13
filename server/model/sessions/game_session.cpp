#include "game_session.h"
cavoke::server::model::game_session_error::game_session_error(
    std::string message)
    : cavoke_base_exception(std::move(message),
                            InvalidClientInput,
                            "cavoke/sessions") {
}
cavoke::server::model::GameSession::GameSession(
    cavoke::server::model::GameConfig game_config,
    std::optional<json> game_settings)
    : id(drogon::utils::getUuid()),
      m_game_config(std::move(game_config)),
      m_invite_code(generate_invite_code()) {
    if (game_settings.has_value()) {
        m_game_settings = game_settings.value();
    } else {
        m_game_settings = m_game_config.default_settings;
    }
}
/**
 * Adds given user to session.
 * Throws an exception if user already in this session or too many players in a
 * session
 */
void cavoke::server::model::GameSession::add_user(const std::string &user_id) {
    // TODO: thread-safety
    if (m_userid_to_playerid.left.count(user_id) != 0) {
        return;
    }
    // get player id for user
    int pos = static_cast<int>(m_userid_to_playerid.size());
    if (pos >= m_game_config.players_num) {
        throw game_session_error("maximum number of players reached");
    }
    // add user to session
    auto [_, suc] = m_userid_to_playerid.insert({user_id, pos});
    std::cout << suc << std::endl;
}
/**
 * Gets player id for user.
 * Throws an exception if user not in this session
 */
int cavoke::server::model::GameSession::get_player_id(
    const std::string &user_id) const {
    try {
        return m_userid_to_playerid.left.at(user_id);
    } catch (const std::out_of_range &) {  // slow?
        throw game_session_error("user not in session");
    }
}
/**
 * Gets user id for player.
 * Throws an exception if player not in this session
 */
std::string cavoke::server::model::GameSession::get_user_id(
    int player_id) const {
    try {
        return m_userid_to_playerid.right.at(player_id);
    } catch (const std::out_of_range &) {  // slow?
        throw game_session_error("player not in session");
    }
}
/// Validates the invite code for this session
bool cavoke::server::model::GameSession::verify_invite_code(
    const std::string &invite_code) const {
    return invite_code == m_invite_code;
}
/// Generates an info object (representation for client)
cavoke::server::model::GameSession::GameSessionInfo
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
    std::generate(res.begin(), res.end(),
                  [&dist, &engine]() { return dist(engine); });
    return res;
}

std::vector<int> cavoke::server::model::GameSession::get_occupied_positions()
    const {
    std::vector<int> result;
    for (const auto &e : m_userid_to_playerid) {
        // cppcheck-suppress useStlAlgorithm
        result.push_back(e.right);
    }
    return result;
}

const cavoke::server::model::json &
cavoke::server::model::GameSession::get_game_settings() const {
    return m_game_settings;
}
