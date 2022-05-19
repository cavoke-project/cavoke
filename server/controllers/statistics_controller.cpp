#include "statistics_controller.h"

namespace cavoke::server::controllers {

StatisticsController::StatisticsController(
    std::shared_ptr<model::GamesStorage> mGamesStorage,
    std::shared_ptr<model::StatisticsManager> mStatisticsManager)
    : m_games_storage(std::move(mGamesStorage)),
      m_statistics_manager(std::move(mStatisticsManager)) {
}

void StatisticsController::game_statistics(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &game_id) {
    if (!m_games_storage->get_game_by_id(game_id).has_value()) {
        return CALLBACK_STATUS_CODE(k404NotFound);
    }

    return callback(newNlohmannJsonResponse(
        m_statistics_manager->get_game_statistics(game_id)));
}

void StatisticsController::user_statistics(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &user_id) {
    std::optional<std::string> opt_game_id =
        req->getOptionalParameter<std::string>("game_id");

    if (opt_game_id.has_value()) {
        const std::string &game_id = opt_game_id.value();
        if (!m_games_storage->get_game_by_id(game_id).has_value()) {
            return CALLBACK_STATUS_CODE(k404NotFound);
        }

        return callback(newNlohmannJsonResponse(
            m_statistics_manager->get_user_game_statistics(user_id, game_id)));
    } else {
        return callback(newNlohmannJsonResponse(
            m_statistics_manager->get_user_statistics(user_id)));
    }
}

}  // namespace cavoke::server::controllers