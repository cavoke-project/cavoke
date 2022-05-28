#include "logic_controller.h"
#include <nlohmann/json.hpp>
#include "../cavoke.h"
#include "../utils.h"

namespace cavoke::logic_server::controllers {

using json = nlohmann::json;

void LogicController::validate(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    try {
        InitSettings settings = json::parse(req->getBody());
        ValidationResult result;

        cavoke::validate_settings(settings.settings,
                                  settings.occupied_positions,
                                  [&result](const std::string &message) {
                                      result.message = message;
                                  });
        callback(newNlohmannJsonResponse(result));
    } catch (const json::parse_error &) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    } catch (const json::out_of_range &) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }
}

void LogicController::init_state(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    try {
        InitSettings settings = json::parse(req->getBody());

        GameState result =
            cavoke::init_state(settings.settings, settings.occupied_positions);
        callback(newNlohmannJsonResponse(result));
    } catch (const json::parse_error &) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    } catch (const json::out_of_range &) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }
}

void LogicController::apply_move(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    try {
        GameMove move = json::parse(req->getBody());

        GameState result = cavoke::apply_move(move);
        callback(newNlohmannJsonResponse(result));
    } catch (const json::parse_error &) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    } catch (const json::out_of_range &) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }
}

}  // namespace cavoke::logic_server::controllers
