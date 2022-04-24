#include "game_state_storage.h"
#include <drogon/HttpAppFramework.h>
#include <json/reader.h>
#include <utility>
#include "sql-models/Globalstates.h"
#include "sql-models/Players.h"
#include "utils.h"

namespace cavoke::server::model {

using json = nlohmann::json;
using namespace drogon::orm;

void GameStateStorage::save_state(const std::string &session_id,
                                  GameStateStorage::GameState new_state) {
    auto mp_globalstates = MAPPER_FOR(drogon_model::cavoke_orm::Globalstates);
    auto mp_players = MAPPER_FOR(drogon_model::cavoke_orm::Players);

    auto global_state = mp_globalstates.findOne(
        Criteria(drogon_model::cavoke_orm::Globalstates::Cols::_session_id,
                 CompareOperator::EQ, session_id));
    auto players = mp_players.findBy(
        Criteria(drogon_model::cavoke_orm::Players::Cols::_session_id,
                 CompareOperator::EQ, session_id));

    global_state.setGlobalstate(new_state.global_state);
    global_state.setIsTerminal(new_state.is_terminal);

    auto transaction = drogon::app().getDbClient()->newTransaction();
    auto mp_sessions_trans = MAPPER_WITH_CLIENT_FOR(
        drogon_model::cavoke_orm::Globalstates, transaction);
    auto mp_players_trans =
        MAPPER_WITH_CLIENT_FOR(drogon_model::cavoke_orm::Players, transaction);

    for (auto &player : players) {
        player.setPlayerstate(
            new_state.players_state[player.getValueOfPlayerId()]);
        if (!new_state.is_terminal) {
            // FIXME: move to a new winner system
            bool is_winner =
                std::any_of(new_state.winners.begin(), new_state.winners.end(),
                            [&](int player_id) {
                                return player.getValueOfPlayerId() == player_id;
                            });
            player.setScore(is_winner ? 1 : 0);
        } else {
            player.setScoreToNull();
        }
        mp_players_trans.update(player);
    }
    mp_sessions_trans.update(global_state);
}

GameStateStorage::GameState GameStateStorage::get_state(
    const std::string &session_id) {
    GameState state;

    auto mp_globalstates = MAPPER_FOR(drogon_model::cavoke_orm::Globalstates);
    auto global_state = mp_globalstates.findOne(drogon::orm::Criteria(
        drogon_model::cavoke_orm::Globalstates::Cols::_session_id,
        drogon::orm::CompareOperator::EQ, session_id));
    state.global_state = global_state.getValueOfGlobalstate();
    state.is_terminal = global_state.getValueOfIsTerminal();

    auto mp_players = MAPPER_FOR(drogon_model::cavoke_orm::Players);
    auto players = mp_players.findBy(drogon::orm::Criteria(
        drogon_model::cavoke_orm::Players::Cols::_session_id,
        drogon::orm::CompareOperator::EQ, session_id));
    state.players_state = std::vector<std::string>(players.size());
    for (auto &player : players) {
        int player_id = player.getValueOfPlayerId();
        if (player.getScore()) {
            state.winners.push_back(player_id);
        }
        state.players_state[player_id] = player.getValueOfPlayerstate();
    }
    return state;
}

std::string GameStateStorage::get_player_state(const std::string &session_id,
                                               int player_id) {
    auto mp_players = MAPPER_FOR(drogon_model::cavoke_orm::Players);
    try {
        auto player = mp_players.findOne(
            Criteria(drogon_model::cavoke_orm::Players::Cols::_session_id,
                     CompareOperator::EQ, session_id) &&
            Criteria(drogon_model::cavoke_orm::Players::Cols::_player_id,
                     CompareOperator::EQ, player_id));
        return player.getValueOfPlayerstate();
    } catch (const std::out_of_range &) {
        throw game_state_error("There's no player '" +
                               std::to_string(player_id) + "' in session '" +
                               session_id + "'");
    }
}

game_state_error::game_state_error(std::string message)
    : cavoke_base_exception(std::move(message), NotFound, "cavoke/state") {
}

}  // namespace cavoke::server::model
