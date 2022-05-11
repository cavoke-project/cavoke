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
    };

    GameStatistics get_game_statistics(const std::string &game_id);
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(StatisticsManager::GameStatistics,
                                   average_duration_sec,
                                   average_players_count);

}  // namespace cavoke::server::model

#endif  // CAVOKE_STATISTICS_MANAGER_H
