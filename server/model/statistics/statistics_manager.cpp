#include "statistics_manager.h"
#include <utility>

namespace cavoke::server::model {

StatisticsManager::StatisticsManager(
    std::shared_ptr<SessionsStorage> mSessionStorage,
    std::shared_ptr<GamesStorage> mGamesStorage)
    : m_session_storage(std::move(mSessionStorage)),
      m_games_storage(std::move(mGamesStorage)) {
}

StatisticsManager::GameStatistics StatisticsManager::get_game_statistics(
    const std::string &game_id) {
    return {get_avg_duration(game_id), get_avg_players_cnt(game_id),
            get_total_time_played(game_id), get_total_games_played(game_id)};
}

int StatisticsManager::get_avg_duration(const std::string &game_id) {
    auto duration_result = drogon::app().getDbClient()->execSqlSync(
        "select get_average_session_time_sec($1::varchar) as res;", game_id);
    return duration_result[0]["res"].as<int>();
}

int StatisticsManager::get_total_time_played(const std::string &game_id) {
    auto duration_result = drogon::app().getDbClient()->execSqlSync(
        "select get_total_time_sec($1::varchar) as res;", game_id);
    return duration_result[0]["res"].as<int>();
}

int StatisticsManager::get_total_games_played(const std::string &game_id) {
    auto duration_result = drogon::app().getDbClient()->execSqlSync(
        "select get_sessions_num($1::varchar) as res;", game_id);
    return duration_result[0]["res"].as<int>();
}

int StatisticsManager::get_avg_players_cnt(const std::string &game_id) {
    auto duration_result = drogon::app().getDbClient()->execSqlSync(
        "select get_average_players_num($1::varchar) as res;", game_id);
    return duration_result[0]["res"].as<int>();
}

}  // namespace cavoke::server::model
