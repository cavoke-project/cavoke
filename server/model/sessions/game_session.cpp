#include "game_session.h"

namespace cavoke::server::model {
using namespace drogon::orm;
using namespace drogon_model::cavoke_test;

game_session_error::game_session_error(std::string message)
    : cavoke_base_exception(std::move(message),
                            InvalidClientInput,
                            "cavoke/sessions") {
}

/**
 * Adds given user to session.
 * Throws an exception if user already in this session, session has already
 * started or too many players in a session
 */
void GameSessionAccessObject::add_user(const std::string &user_id,
                                       std::optional<int> player_id) {
    auto session_snapshot = get_snapshot();
    if (session_snapshot.getValueOfStatus() != NOT_STARTED) {
        throw game_session_error("session has already started");
    }

    if (0 !=
        default_mp_players.count(
            Criteria(Players::Cols::_session_id, CompareOperator::EQ, id) &&
            Criteria(Players::Cols::_user_id, CompareOperator::EQ, user_id))) {
        return;
    }
    // get player id for user
    int pos;
    if (player_id.has_value()) {
        pos = player_id.value();
    } else {
        std::set<int> possible_positions;
        for (int candidate_pos = 0; candidate_pos < m_game_config.players_num;
             ++candidate_pos) {
            possible_positions.insert(candidate_pos);
        }
        for (int occupied_pos : get_occupied_positions()) {
            possible_positions.erase(occupied_pos);
        }
        if (possible_positions.empty()) {
            throw game_session_error("maximum number of players reached");
        }
        pos = *possible_positions.begin();
    }
    try {
        Players new_player;
        new_player.setUserId(user_id);
        new_player.setSessionId(id);
        new_player.setScoreToNull();
        new_player.setPlayerId(pos);
        new_player.setPlayerstate("");
        default_mp_players.insert(new_player);
        LOG_DEBUG << "Added: " << user_id << " with player_id=" << pos << " to "
                  << id;
    } catch (const DrogonDbException &) {
        throw game_session_error("position is already occupied");
    }
}

/**
 * Gets player id for user.
 * Throws an exception if user not in this session
 */
int GameSessionAccessObject::get_player_id(const std::string &user_id) const {
    try {
        return default_mp_players
            .findOne(
                Criteria(Players::Cols::_session_id, CompareOperator::EQ, id) &&
                Criteria(Players::Cols::_user_id, CompareOperator::EQ, user_id))
            .getValueOfPlayerId();
    } catch (const UnexpectedRows &) {
        throw game_session_error("user not in session");
    }
}

/**
 * Gets user id for player.
 * Throws an exception if player not in this session
 */
std::string GameSessionAccessObject::get_user_id(int player_id) const {
    try {
        return default_mp_players
            .findOne(
                Criteria(Players::Cols::_session_id, CompareOperator::EQ, id) &&
                Criteria(Players::Cols::_player_id, CompareOperator::EQ,
                         player_id))
            .getValueOfUserId();
    } catch (const UnexpectedRows &) {
        throw game_session_error("user not in session");
    }
}

/// Validates the invite code for this session
bool GameSessionAccessObject::verify_invite_code(
    const std::string &invite_code) const {
    auto session = get_snapshot();
    return invite_code == session.getValueOfInviteCode();
}

/// Generates an info object (representation for client)
GameSessionAccessObject::GameSessionInfo
GameSessionAccessObject::get_session_info() const {
    auto session_snapshot = get_snapshot();
    return make_session_info(session_snapshot, get_players());
}

std::vector<int> GameSessionAccessObject::get_occupied_positions() const {
    std::vector<int> result;
    for (const auto &player : default_mp_players.findBy(
             Criteria(Players::Cols::_session_id, CompareOperator::EQ, id))) {
        // cppcheck-suppress useStlAlgorithm
        result.push_back(player.getValueOfPlayerId());
    }
    return result;
}

// std::optional<json> &GameSessionAccessObject::get_game_settings() const {
//     auto session = get_snapshot();
//     auto settings_ptr = session.getGameSettings();
//     return *settings_ptr;
// }

std::vector<GameSessionAccessObject::PlayerInfo>
GameSessionAccessObject::get_players() const {
    auto players = default_mp_players.findBy(
        Criteria(Players::Cols::_session_id, CompareOperator::EQ, id));
    std::vector<PlayerInfo> result;
    std::transform(players.begin(), players.end(), std::back_inserter(result),
                   [](const Players &player) {
                       return PlayerInfo{player.getValueOfUserId(),
                                         player.getValueOfPlayerId()};
                   });
    return result;
}

void GameSessionAccessObject::start(const json &game_settings) {
    auto session = default_mp_sessions.findOne(
        Criteria(Sessions::Cols::_id, CompareOperator::EQ, id));
    // FIXME: not atomic, transactions perhaps or some other blocking
    // sql-mechanism?
    if (session.getValueOfStatus() != NOT_STARTED) {
        throw game_session_error("session has already started");
    }
    session.setGameSettings(game_settings.dump());
    session.setStatus(RUNNING);
    default_mp_sessions.update(session);
}

bool GameSessionAccessObject::is_player(const std::string &user_id) const {
    try {
        int tmp = get_player_id(user_id);
        return true;
    } catch (const std::out_of_range &) {
        return false;
    }
}

void GameSessionAccessObject::finish() {
    auto session = default_mp_sessions.findOne(
        Criteria(Sessions::Cols::_id, CompareOperator::EQ, id));
    session.setStatus(FINISHED);
    default_mp_sessions.update(session);
}
drogon_model::cavoke_test::Sessions GameSessionAccessObject::get_snapshot()
    const {
    return default_mp_sessions.findOne(
        Criteria(Sessions::Cols::_id, CompareOperator::EQ, id));
}
drogon_model::cavoke_test::Sessions GameSessionAccessObject::get_snapshot(
    const std::string &session_id) {
    auto mp_sessions = MAPPER_FOR(drogon_model::cavoke_test::Sessions);
    return mp_sessions.findOne(
        Criteria(drogon_model::cavoke_test::Sessions::Cols::_id,
                 drogon::orm::CompareOperator::EQ, session_id));
}

GameSessionAccessObject::GameSessionInfo
GameSessionAccessObject::make_session_info(const Sessions &session,
                                           std::vector<PlayerInfo> players) {
    return {session.getValueOfId(), session.getValueOfGameId(),
            session.getValueOfInviteCode(),
            static_cast<SessionStatus>(session.getValueOfStatus()),
            std::move(players)};
}

}  // namespace cavoke::server::model
