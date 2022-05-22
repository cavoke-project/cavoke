#ifndef CAVOKE_STATISTICS_CONTROLLER_H
#define CAVOKE_STATISTICS_CONTROLLER_H

#include "drogon/HttpController.h"
#include "model/games/games_storage.h"
#include "model/statistics/statistics_manager.h"

namespace cavoke::server::controllers {

class StatisticsController
    : public drogon::HttpController<StatisticsController, false> {
    std::shared_ptr<model::GamesStorage> m_games_storage;
    std::shared_ptr<model::StatisticsManager> m_statistics_manager;

public:
    StatisticsController(
        std::shared_ptr<model::GamesStorage> mGamesStorage,
        std::shared_ptr<model::StatisticsManager> mStatisticsManager);

public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(StatisticsController::game_statistics,
                  "/statistics/game/{game_id}",
                  drogon::Get);
    ADD_METHOD_TO(StatisticsController::user_statistics,
                  "/statistics/user/{user_id}",
                  drogon::Get);
    ADD_METHOD_TO(StatisticsController::user_game_statistics,
                  "/statistics/user/{user_id}/game/{game_id}",
                  drogon::Get);
    ADD_METHOD_TO(StatisticsController::my_user_statistics,
                  "/profile/my_user_statistics",
                  drogon::Get,
                  "AuthFilter");
    ADD_METHOD_TO(StatisticsController::my_user_game_statistics,
                  "/profile/my_user_game_statistics/{game_id}",
                  drogon::Get,
                  "AuthFilter");
    METHOD_LIST_END

protected:
    void game_statistics(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback,
        const std::string &game_id);

    void user_statistics(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback,
        const std::string &user_id);

    void user_game_statistics(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback,
        const std::string &user_id,
        const std::string &game_id);

    void my_user_statistics(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);

    void my_user_game_statistics(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback,
        const std::string &game_id);
};

}  // namespace cavoke::server::controllers

#endif  // CAVOKE_STATISTICS_CONTROLLER_H
