#include "games_controller.h"
#include <boost/filesystem/operations.hpp>
#include <utility>
#include "../model/game.h"

cavoke::server::controllers::GamesController::GamesController(
    std::shared_ptr<model::GamesStorage> games_storage)
    : m_games_storage(std::move(games_storage)) {
}

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

void cavoke::server::controllers::GamesController::game_config(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &game_id) {
    auto game = m_games_storage->get_game_by_id(game_id);
    if (!game.has_value()) {
        auto resp = drogon::HttpResponse::newNotFoundResponse();
        callback(resp);
        return;
    }

    auto resp =
        drogon::HttpResponse::newHttpJsonResponse(game->config.to_json());
    callback(resp);
}

void cavoke::server::controllers::GamesController::game_client_file(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &game_id) {
    auto game = m_games_storage->get_game_by_id(game_id);
    if (!game.has_value()) {
        auto resp = drogon::HttpResponse::newNotFoundResponse();
        callback(resp);
        return;
    }

    auto resp = drogon::HttpResponse::newFileResponse(
        boost::filesystem::canonical(game->client_file).string(),
        game->config.id + ".zip");
    callback(resp);
}
