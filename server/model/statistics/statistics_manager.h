#ifndef CAVOKE_STATISTICS_MANAGER_H
#define CAVOKE_STATISTICS_MANAGER_H

#include <memory>
#include "model/sessions/sessions_storage.h"

namespace cavoke::server::model {

class StatisticsManager {
    std::shared_ptr<SessionsStorage> m_session_storage;
    std::shared_ptr<GamesStorage> m_games_storage;

public:
    explicit StatisticsManager(std::shared_ptr<SessionsStorage> mSessionStorage,
                               std::shared_ptr<GamesStorage> mGamesStorage);

    struct GameStatistics {
        int average_duration_sec;
        int average_players_count;
        int total_time_played_sec;
        int total_games_played;
    };

    GameStatistics get_game_statistics(const std::string &game_id);

private:
    int get_avg_duration(const std::string &game_id);
    int get_avg_players_cnt(const std::string &game_id);
    int get_total_time_played(const std::string &game_id);
    int get_total_games_played(const std::string &game_id);
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(StatisticsManager::GameStatistics,
                                   average_duration_sec,
                                   average_players_count,
                                   total_time_played_sec,
                                   total_games_played);

}  // namespace cavoke::server::model

#endif  // CAVOKE_STATISTICS_MANAGER_H
