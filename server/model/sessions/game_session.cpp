#include "game_session.h"

namespace cavoke::server::model {

game_session_error::game_session_error(std::string message)
    : cavoke_base_exception(std::move(message),
                            InvalidClientInput,
                            "cavoke/sessions") {
}

GameSession::GameSession(GameConfig game_config)
    : id(drogon::utils::getUuid()),
      m_game_config(std::move(game_config)),
      m_status(NOT_STARTED),
      m_invite_code(generate_invite_code()) {
}

/**
 * Adds given user to session.
 * Throws an exception if user already in this session, session has already
 * started or too many players in a session
 */
void GameSession::add_user(const std::string &user_id) {
    // TODO: thread-safety
    if (m_status != NOT_STARTED) {
        throw game_session_error("session has already started");
    }

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
int GameSession::get_player_id(const std::string &user_id) const {
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
std::string GameSession::get_user_id(int player_id) const {
    try {
        return m_userid_to_playerid.right.at(player_id);
    } catch (const std::out_of_range &) {  // slow?
        throw game_session_error("player not in session");
    }
}

/// Validates the invite code for this session
bool GameSession::verify_invite_code(const std::string &invite_code) const {
    return invite_code == m_invite_code;
}

/// Generates an info object (representation for client)
GameSession::GameSessionInfo GameSession::get_session_info() const {
    return {id, m_game_config.id, m_invite_code, m_status,
            std::move(get_players())};
}

/// Generates an invite code for session
std::string GameSession::generate_invite_code() {
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

std::vector<int> GameSession::get_occupied_positions() const {
    std::vector<int> result;
    for (const auto &e : m_userid_to_playerid) {
        result.push_back(e.right);
    }
    return result;
}

const std::optional<json> &GameSession::get_game_settings() const {
    return m_game_settings;
}

std::vector<GameSession::Player> GameSession::get_players() const {
    std::vector<Player> result;
    for (const auto &e : m_userid_to_playerid) {
        result.push_back({e.left, e.right});
    }
    return result;
}

void GameSession::start(const json &game_settings) {
    if (m_status != NOT_STARTED) {
        throw game_session_error("session has already started");
    }
    m_game_settings = game_settings;
    m_status = RUNNING;
}

}  // namespace cavoke::server::model
