#include "game_logic_manager.h"
// workaround for a boost bug...
// https://github.com/boostorg/process/issues/96
#ifndef __kernel_entry
#define __kernel_entry
#endif
#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <utility>

namespace cavoke::server::model {

GameLogicManager::GameLogicManager(std::shared_ptr<GamesStorage> games_storage)
    : m_games_storage(std::move(games_storage)) {
}

std::string GameLogicManager::invoke_logic_local(const Game &game,
                                                 const std::string &input) {
    namespace bp = boost::process;
    boost::asio::io_service ios;

    auto input_buffer = boost::asio::buffer(input);
    bp::async_pipe input_pipe(ios);
    std::vector<char> output_buffer(MAX_GAME_RESPONSE_B);

    bp::child c(
        system_complete(game.logic_file), bp::start_dir(game.directory),
        bp::std_in<input_pipe, bp::std_out> boost::asio::buffer(output_buffer),
        ios);

    boost::asio::async_write(
        input_pipe, input_buffer,
        [&input_pipe](const boost::system::error_code &ec, std::size_t n) {
            input_pipe.async_close();
        });

    ios.run();

    if (!c.wait_for(std::chrono::milliseconds{LOGIC_TIMEOUT_MS})) {
        LOG_ERROR << game.logic_file.string() << " logic executable timeout ("
                  << LOGIC_TIMEOUT_MS << " ms) exceeded";
        c.terminate();
    }

    return std::string(output_buffer.begin(), output_buffer.end());
}

cavoke::ValidationResult GameLogicManager::validate_settings(
    const std::string &game_id,
    const json &settings,
    const std::vector<int> &occupied_positions) {
    std::optional<Game> game_info = m_games_storage->get_game_by_id(game_id);
    if (!game_info.has_value()) {
        throw validation_error("no game with id '" + game_id + "'");
    }

    std::string response;

    if (game_info->config.app_type == "LOCAL") {
        std::ostringstream request;
        request << "VALIDATE ";
        json request_data = InitSettings{settings, occupied_positions};
        request << request_data;
        response = invoke_logic_local(game_info.value(), request.str());
    } else if (game_info->config.app_type == "EXTERNAL") {
        response = invoke_logic_external(
            game_info.value(), "validate",
            json(InitSettings{settings, occupied_positions}).dump());
    }

    json response_json = json::parse(response);
    auto result = response_json.get<ValidationResult>();

    return result;
}

GameStateStorage::GameState GameLogicManager::init_state(
    const std::string &game_id,
    const json &settings,
    const std::vector<int> &occupied_positions) {
    std::optional<Game> game_info = m_games_storage->get_game_by_id(game_id);
    if (!game_info.has_value()) {
        return {};
    }

    std::string response;

    if (game_info->config.app_type == "LOCAL") {
        std::ostringstream request;
        request << "INIT ";
        json request_data = InitSettings{settings, occupied_positions};
        request << request_data;
        response = invoke_logic_local(game_info.value(), request.str());
    } else if (game_info->config.app_type == "EXTERNAL") {
        response = invoke_logic_external(
            game_info.value(), "init_state",
            json(InitSettings{settings, occupied_positions}).dump());
    }

    json response_json = json::parse(response);
    auto result = response_json.get<GameStateStorage::GameState>();

    return result;
}

GameStateStorage::GameState GameLogicManager::send_move(
    const std::string &game_id,
    const cavoke::GameMove &move) {
    std::optional<Game> game_info = m_games_storage->get_game_by_id(game_id);
    if (!game_info.has_value()) {
        return {};
    }

    std::string response;

    if (game_info->config.app_type == "LOCAL") {
        std::ostringstream request;
        request << "MOVE ";
        json request_data = move;
        request << request_data;
        response = invoke_logic_local(game_info.value(), request.str());
    } else if (game_info->config.app_type == "EXTERNAL") {
        response = invoke_logic_external(game_info.value(), "apply_move",
                                         json(move).dump());
    }

    json response_json = json::parse(response);
    auto result = response_json.get<GameStateStorage::GameState>();

    return result;
}

std::string GameLogicManager::invoke_logic_external(const Game &game,
                                                    const std::string &method,
                                                    const std::string &input) {
    auto client = drogon::HttpClient::newHttpClient(game.config.url,
                                                    drogon::app().getLoop());
    auto request = drogon::HttpRequest::newHttpRequest();
    request->setPath("/" + method);
    request->setMethod(drogon::Post);
    request->setBody(input);
    auto result = client->sendRequest(request, 30.0);

    if (result.first != drogon::ReqResult::Ok) {
        LOG_ERROR << "Failed to connect with game logic on address "
                  << game.config.url << " " << request->getPath() << ": "
                  << to_string(result.first);
        return "";
    }

    return std::string(result.second->getBody());
}

validation_error::validation_error(std::string message)
    : cavoke_base_exception(std::move(message),
                            InvalidConfig,
                            "cavoke/validation") {
}

}  // namespace cavoke::server::model
