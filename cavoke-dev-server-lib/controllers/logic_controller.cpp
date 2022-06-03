#include "logic_controller.h"
#include <nlohmann/json.hpp>
#include "../cavoke.h"
#include "../utils.h"

namespace cavoke::logic_server::controllers {

using json = nlohmann::json;

void LogicController::validate(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    std::cout << "GOT REQUEST " << req->getBody() << std::endl;
    try {
        InitSettings settings = json::parse(req->getBody());
        ValidationResult result;

        result.success = cavoke::validate_settings(
            settings.settings, settings.occupied_positions,
            [&result](const std::string &message) {
                result.message = message;
            });

        std::cout << "SENT RESPONSE " << json(result).dump() << std::endl;
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
    std::cout << "GOT REQUEST " << req->getBody() << std::endl;
    try {
        InitSettings settings = json::parse(req->getBody());

        GameState result =
            cavoke::init_state(settings.settings, settings.occupied_positions);

        std::cout << "SENT RESPONSE " << json(result).dump() << std::endl;
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
    std::cout << "GOT REQUEST " << req->getBody() << std::endl;
    try {
        GameMove move = json::parse(req->getBody());

        GameState result = cavoke::apply_move(move);

        std::cout << "SENT RESPONSE " << json(result).dump() << std::endl;
        callback(newNlohmannJsonResponse(result));
    } catch (const json::parse_error &) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    } catch (const json::out_of_range &) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }
}

}  // namespace cavoke::logic_server::controllers
