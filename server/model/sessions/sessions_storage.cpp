#include "sessions_storage.h"
#include <utility>
#include "sql-models/Globalstates.h"
#include "sql-models/Sessions.h"
#include "sql-models/Statuses.h"

namespace cavoke::server::model {

using namespace drogon::orm;

/**
 * Creates a session with given game config.
 *
 * Throws `game_session_error` if errors arise
 *
 * @return session info
 */
GameSessionAccessObject::GameSessionInfo SessionsStorage::create_session(
    const GameConfig &game_config,
    const std::string &host_user_id) {
    // create session
    auto session = drogon_model::cavoke_orm::Sessions();
    {
        session.setId(drogon::utils::getUuid());
        session.setGameSettingsToNull();
        session.setGameId(game_config.id);
        // TODO: there are only 1e6 invite codes, something has to be done about
        session.setInviteCode(generate_invite_code());
    }
    auto session_status = drogon_model::cavoke_orm::Statuses();
    {
        session_status.setStatus(
            GameSessionAccessObject::SessionStatus::NOT_STARTED);
        session_status.setSessionId(session.getValueOfId());
    }
    auto host_player = drogon_model::cavoke_orm::Players();
    {
        // TODO: should we always add host on 0 position?
        host_player.setPlayerId(0);
        host_player.setPlayerstate("");
        host_player.setScoreToNull();
        host_player.setSessionId(session.getValueOfId());
        host_player.setUserId(host_user_id);
    }
    auto global_state = drogon_model::cavoke_orm::Globalstates();
    {
        global_state.setSessionId(session.getValueOfId());
        global_state.setIsTerminal(false);
        global_state.setGlobalstateToNull();
    }

    {
        auto transaction = drogon::app().getDbClient()->newTransaction();
        auto mp_sessions = MAPPER_WITH_CLIENT_FOR(
            drogon_model::cavoke_orm::Sessions, transaction);
        mp_sessions.insert(session);
        auto mp_statuses = MAPPER_WITH_CLIENT_FOR(
            drogon_model::cavoke_orm::Statuses, transaction);
        mp_statuses.insert(session_status);
        auto mp_players = MAPPER_WITH_CLIENT_FOR(
            drogon_model::cavoke_orm::Players, transaction);
        mp_players.insert(host_player);
        auto mp_globalstates = MAPPER_WITH_CLIENT_FOR(
            drogon_model::cavoke_orm::Globalstates, transaction);
        mp_globalstates.insert(global_state);

        session.setHostId(host_user_id);
        mp_sessions.update(session);
    }
    LOG_DEBUG << "Session created: " << session.getValueOfId();
    // TODO: cleanup and use GameSessionAccessObject's non-static methods
    return GameSessionAccessObject::make_session_info(session, session_status,
                                                      {{host_user_id, 0}});
}

void SessionsStorage::start_session(const std::string &session_id,
                                    std::optional<json> game_settings) {
    auto session = get_sessionAO(session_id);
    auto validation_result = validate_session(session_id, game_settings);

    if (!validation_result.success) {
        throw validation_error(validation_result.message);
    }
    std::string game_id = session.get_session_info().game_id;
    if (!game_settings.has_value()) {
        game_settings = m_games_storage->get_game_by_id(game_id)
                            .value()
                            .config.default_settings;
    }
    // Lock joining
    session.start(game_settings.value());
    LOG_DEBUG << "Session started: " << session_id;
    // Initialize state
    m_game_state_storage->save_state(
        session_id,
        m_game_logic_manager->init_state(session.get_session_info().game_id,
                                         game_settings.value(),
                                         session.get_occupied_positions()));
}

/**
 * Joins a session by invite code
 *
 * Throws `game_session_error` if errors arise
 *
 * @return session info
 */
GameSessionAccessObject::GameSessionInfo SessionsStorage::join_session(
    const std::string &invite_code,
    const std::string &user_id,
    std::optional<int> player_id) {
    auto sessionAO = get_sessionAO_by_invite(invite_code);
    // add the user
    sessionAO.add_user(user_id, player_id);
    // generate representation for client
    return sessionAO.get_session_info();
}

SessionsStorage::SessionsStorage(
    std::shared_ptr<GameLogicManager> mGameLogicManager,
    std::shared_ptr<GamesStorage> mGamesStorage,
    std::shared_ptr<GameStateStorage> mGameStateStorage)
    : m_game_logic_manager(std::move(mGameLogicManager)),
      m_games_storage(std::move(mGamesStorage)),
      m_game_state_storage(std::move(mGameStateStorage)) {
}
cavoke::ValidationResult SessionsStorage::validate_session(
    const std::string &session_id,
    std::optional<json> game_settings) {
    auto session = get_sessionAO(session_id);
    // FIXME: 2 reads happen not simultaneously
    std::string game_id = session.get_session_info().game_id;
    if (!game_settings.has_value()) {
        game_settings = m_games_storage->get_game_by_id(game_id)
                            .value()
                            .config.default_settings;
    }

    return m_game_logic_manager->validate_settings(
        game_id, game_settings.value(),
        session.get_occupied_positions());  // FIXME: 2 reads happen not
                                            // simultaneously
}

/// Generates an invite code for session
std::string SessionsStorage::generate_invite_code() {
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
/**
 * Gets a session for given session id
 *
 * Throws `game_session_error` if no such session
 */
GameSessionAccessObject SessionsStorage::get_sessionAO(
    const std::string &session_id) {
    // INFO: redundancy to check that session actually exists
    try {
        auto mp_sessions = MAPPER_FOR(
            drogon_model::cavoke_orm::Sessions);  // TODO: use shared mappers
        // find session by invite
        auto session = mp_sessions.findOne(
            Criteria(drogon_model::cavoke_orm::Sessions::Cols::_id,
                     CompareOperator::EQ, session_id));
        return GameSessionAccessObject(
            session.getValueOfId(),
            m_games_storage->get_game_by_id(session.getValueOfGameId())
                ->config);
    } catch (const std::out_of_range &) {
        throw game_session_error("session does not exist: '" + session_id +
                                 "'");
    }
}
GameSessionAccessObject SessionsStorage::get_sessionAO_by_invite(
    const std::string &invite_code) {
    try {
        auto mp_sessions = MAPPER_FOR(
            drogon_model::cavoke_orm::Sessions);  // TODO: use shared mappers
        // find session by invite
        auto session = mp_sessions.findOne(
            Criteria(drogon_model::cavoke_orm::Sessions::Cols::_invite_code,
                     CompareOperator::EQ, invite_code));
        return GameSessionAccessObject(
            session.getValueOfId(),
            m_games_storage->get_game_by_id(session.getValueOfGameId())
                ->config);
    } catch (const std::out_of_range &) {
        throw game_session_error("invite code does not exist: '" + invite_code +
                                 "'");
    }
}

}  // namespace cavoke::server::model
