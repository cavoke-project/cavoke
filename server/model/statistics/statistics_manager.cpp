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
    auto duration_result = drogon::app().getDbClient()->execSqlSync(
        "select get_average_session_time_sec($1::varchar) as res;", game_id);
    int avg_duration = duration_result[0]["res"].as<int>();
    return {avg_duration, 2};
}

}  // namespace cavoke::server::model
