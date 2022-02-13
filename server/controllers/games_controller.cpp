#include "games_controller.h"
#include "../model/game.h"

#include <utility>

cavoke::server::controllers::GamesController::GamesController(
    std::shared_ptr<model::GamesStorage> games_storage)
    : m_games_storage(std::move(games_storage)) {}

void cavoke::server::controllers::GamesController::games_list(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
  std::vector<model::Game> games = m_games_storage->list_games();
  Json::Value result(Json::arrayValue);
  for (const auto &game : games) {
    result.append(game.config.to_json());
  }

  auto resp = drogon::HttpResponse::newHttpJsonResponse(result);
  callback(resp);
}
