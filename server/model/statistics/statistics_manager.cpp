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

StatisticsManager::UserGameStatistics
StatisticsManager::get_user_game_statistics(const std::string &user_id,
                                            const std::string &game_id) {
    return {game_id, get_user_time_played(user_id, game_id),
            get_user_games_played(user_id, game_id),
            get_user_win_rate(user_id, game_id)};
}

StatisticsManager::UserStatistics StatisticsManager::get_user_statistics(
    const std::string &user_id) {
    return {get_user_total_time_played(user_id),
            get_user_total_games_played(user_id)};
}

int StatisticsManager::get_avg_duration(const std::string &game_id) {
    auto db_result = drogon::app().getDbClient()->execSqlSync(
        "select get_average_session_time_sec($1::varchar) as res;", game_id);
    return db_result[0]["res"].as<int>();
}

int StatisticsManager::get_total_time_played(const std::string &game_id) {
    auto db_result = drogon::app().getDbClient()->execSqlSync(
        "select get_total_time_sec($1::varchar) as res;", game_id);
    return db_result[0]["res"].as<int>();
}

int StatisticsManager::get_total_games_played(const std::string &game_id) {
    auto db_result = drogon::app().getDbClient()->execSqlSync(
        "select get_sessions_num($1::varchar) as res;", game_id);
    return db_result[0]["res"].as<int>();
}

int StatisticsManager::get_avg_players_cnt(const std::string &game_id) {
    auto db_result = drogon::app().getDbClient()->execSqlSync(
        "select get_average_players_num($1::varchar) as res;", game_id);
    return db_result[0]["res"].as<int>();
}

int StatisticsManager::get_user_total_time_played(const std::string &user_id) {
    auto db_result = drogon::app().getDbClient()->execSqlSync(
        "select get_cavoke_time_sec_for_user($1::varchar) as res;", user_id);
    return db_result[0]["res"].as<int>();
}

int StatisticsManager::get_user_total_games_played(const std::string &user_id) {
    auto db_result = drogon::app().getDbClient()->execSqlSync(
        "select get_cavoke_sessions_num_for_user($1::varchar) as res;",
        user_id);
    return db_result[0]["res"].as<int>();
}

double StatisticsManager::get_user_win_rate(const std::string &user_id,
                                            const std::string &game_id) {
    auto db_result = drogon::app().getDbClient()->execSqlSync(
        "select get_win_rate($1::varchar, $2::varchar) as res;", game_id,
        user_id);
    return db_result[0]["res"].as<double>();
}

int StatisticsManager::get_user_games_played(const std::string &user_id,
                                             const std::string &game_id) {
    auto db_result = drogon::app().getDbClient()->execSqlSync(
        "select get_sessions_num_for_user($1::varchar, $2::varchar) as res;",
        game_id, user_id);
    return db_result[0]["res"].as<int>();
}

int StatisticsManager::get_user_time_played(const std::string &user_id,
                                            const std::string &game_id) {
    auto db_result = drogon::app().getDbClient()->execSqlSync(
        "select get_total_time_sec_for_user($1::varchar, $2::varchar) as res;",
        game_id, user_id);
    return db_result[0]["res"].as<int>();
}

}  // namespace cavoke::server::model
