#include "game_session.h"

namespace cavoke::server::model {
using namespace drogon::orm;

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
    if (get_actual_status().getValueOfStatus() != NOT_STARTED) {
        throw game_session_error("session has already started");
    }

    if (0 != default_mp_players.count(
                 Criteria(drogon_model::cavoke_orm::Players::Cols::_session_id,
                          CompareOperator::EQ, id) &&
                 Criteria(drogon_model::cavoke_orm::Players::Cols::_user_id,
                          CompareOperator::EQ, user_id))) {
        return;
    }
    // get player id for user
    int pos;
    if (player_id.has_value()) {
        pos = player_id.value();
        // check role's validity
        if (!(0 <= pos && pos < m_game_config.players_num)) {
            throw game_session_error("no such role " + std::to_string(pos));
        }
        // NOTE: same player_ids are not possible thanks to a constraint in sql
        // schema
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
        drogon_model::cavoke_orm::Players new_player;
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
                Criteria(drogon_model::cavoke_orm::Players::Cols::_session_id,
                         CompareOperator::EQ, id) &&
                Criteria(drogon_model::cavoke_orm::Players::Cols::_user_id,
                         CompareOperator::EQ, user_id))
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
                Criteria(drogon_model::cavoke_orm::Players::Cols::_session_id,
                         CompareOperator::EQ, id) &&
                Criteria(drogon_model::cavoke_orm::Players::Cols::_player_id,
                         CompareOperator::EQ, player_id))
            .getValueOfUserId();
    } catch (const UnexpectedRows &) {
        throw game_session_error("user not in session");
    }
}

/// Generates an info object (representation for client)
GameSessionAccessObject::GameSessionInfo
GameSessionAccessObject::get_session_info() const {
    auto session_snapshot = get_snapshot();
    return make_session_info(session_snapshot, get_actual_status(),
                             get_players());
}

std::vector<int> GameSessionAccessObject::get_occupied_positions() const {
    std::vector<int> result;
    for (const auto &player : default_mp_players.findBy(
             Criteria(drogon_model::cavoke_orm::Players::Cols::_session_id,
                      CompareOperator::EQ, id))) {
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
        Criteria(drogon_model::cavoke_orm::Players::Cols::_session_id,
                 CompareOperator::EQ, id));
    auto users_in_session_result = drogon::app().getDbClient()->execSqlSync(
        "select u.*, player_id from players join users u on players.user_id = "
        "u.id and players.session_id = $1",
        id);
    std::map<int, drogon_model::cavoke_orm::Users> users;
    for (auto &r : users_in_session_result) {
        int player_id = r["player_id"].as<int>();
        users[player_id] = drogon_model::cavoke_orm::Users(r);
    }
    std::vector<PlayerInfo> result;
    std::transform(
        players.begin(), players.end(), std::back_inserter(result),
        [&users](const drogon_model::cavoke_orm::Players &player) {
            int player_id = player.getValueOfPlayerId();
            return PlayerInfo{UserInfo::from_user(users[player_id]), player_id};
        });
    return result;
}

void GameSessionAccessObject::start(const json &game_settings) {
    auto session = default_mp_sessions.findOne(
        Criteria(drogon_model::cavoke_orm::Sessions::Cols::_id,
                 CompareOperator::EQ, id));
    // FIXME: not atomic, transactions perhaps or some other blocking
    // sql-mechanism?
    if (get_actual_status().getValueOfStatus() != NOT_STARTED) {
        throw game_session_error("session has already started");
    }
    session.setGameSettings(game_settings.dump());
    set_status(RUNNING);
    default_mp_sessions.update(session);
}

bool GameSessionAccessObject::is_player(const std::string &user_id) const {
    try {
        [[maybe_unused]] int tmp = get_player_id(user_id);
        return true;
    } catch (const game_session_error &) {
        return false;
    }
}

std::string GameSessionAccessObject::get_host() const {
    return get_snapshot().getValueOfHostId();
}

bool GameSessionAccessObject::is_host(const std::string &user_id) const {
    return get_host() == user_id;
}

void GameSessionAccessObject::finish() {
    set_status(FINISHED);
}
drogon_model::cavoke_orm::Sessions GameSessionAccessObject::get_snapshot()
    const {
    return default_mp_sessions.findOne(
        Criteria(drogon_model::cavoke_orm::Sessions::Cols::_id,
                 CompareOperator::EQ, id));
}
drogon_model::cavoke_orm::Sessions GameSessionAccessObject::get_snapshot(
    const std::string &session_id) {
    auto mp_sessions = MAPPER_FOR(drogon_model::cavoke_orm::Sessions);
    return mp_sessions.findOne(
        Criteria(drogon_model::cavoke_orm::Sessions::Cols::_id,
                 drogon::orm::CompareOperator::EQ, session_id));
}

GameSessionAccessObject::GameSessionInfo
GameSessionAccessObject::make_session_info(
    const drogon_model::cavoke_orm::Sessions &session,
    const drogon_model::cavoke_orm::Statuses &status,
    std::vector<PlayerInfo> players) {
    return {session.getValueOfId(), session.getValueOfGameId(),
            session.getValueOfHostId(),
            static_cast<SessionStatus>(status.getValueOfStatus()),
            std::move(players)};
}

void GameSessionAccessObject::remove_user(const std::string &user_id) {
    if (get_actual_status().getValueOfStatus() != NOT_STARTED) {
        throw game_session_error("session has already started");
    }

    default_mp_players.deleteBy(
        Criteria(drogon_model::cavoke_orm::Players::Cols::_user_id,
                 CompareOperator::EQ, user_id));
}

void GameSessionAccessObject::set_role(const std::string &user_id,
                                       int new_role) {
    // check role's validity
    if (!(0 <= new_role && new_role < m_game_config.players_num)) {
        throw game_session_error("no such role " + std::to_string(new_role));
    }
    try {
        // INFO: does not differentiate the following cases: user not in this
        // session, user in this session changes his role to his current one
        default_mp_players.updateBy(
            {drogon_model::cavoke_orm::Players::Cols::_player_id},
            Criteria(drogon_model::cavoke_orm::Players::Cols::_user_id,
                     CompareOperator::EQ, user_id),
            new_role);
    } catch (const std::exception &err) {
        throw game_session_error("could not update role for player");
    }
}

void GameSessionAccessObject::delete_session() {
    default_mp_sessions.deleteBy(
        Criteria(drogon_model::cavoke_orm::Sessions::Cols::_id,
                 CompareOperator::EQ, id));
}

void GameSessionAccessObject::leave_session(const std::string &user_id) {
    drogon::app().getDbClient()->execSqlSync(
        "select leave_session($1::uuid, $2::varchar);", id, user_id);
}

drogon_model::cavoke_orm::Statuses GameSessionAccessObject::get_actual_status()
    const {
    return default_mp_statuses
        .orderBy(drogon_model::cavoke::Statuses::Cols::_saved_on,
                 SortOrder::DESC)
        .findBy(Criteria(drogon_model::cavoke_orm::Statuses::Cols::_session_id,
                         CompareOperator::EQ, id))[0];
}

void GameSessionAccessObject::set_status(SessionStatus status) {
    drogon_model::cavoke::Statuses status_record;
    auto session = get_snapshot();
    status_record.setSessionId(session.getValueOfId());
    status_record.setStatus(status);
    default_mp_statuses.insert(status_record);
}

}  // namespace cavoke::server::model
