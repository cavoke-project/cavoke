#include "game_logic_manager.h"
#include <boost/process.hpp>
#include <utility>

namespace cavoke::server::model {

GameLogicManager::GameLogicManager(std::shared_ptr<GamesStorage> games_storage)
    : m_games_storage(std::move(games_storage)) {
}

std::string GameLogicManager::invoke_logic(const Game &game,
                                           const std::string &input) {
    namespace bp = boost::process;
    bp::opstream in;
    bp::ipstream out;

    bp::child c(game.logic_file, bp::std_out > out, bp::std_in < in);
    in.write(input.c_str(), static_cast<int>(input.size()));
    in.flush();

    in.pipe().close();

    if (!c.wait_for(std::chrono::milliseconds{500})) {
        c.terminate();
    }

    std::string output;
    std::string line;

    while (std::getline(out, line)) {
        output += line;
        output.push_back('\n');
    }

    return output;
}

GameLogicManager::ValidationResult GameLogicManager::validate_settings(
    const std::string &game_id,
    const json &settings,
    const std::vector<int> &occupied_positions) {
    std::optional<Game> game_info = m_games_storage->get_game_by_id(game_id);
    if (!game_info.has_value()) {
        throw validation_error("no game with id '" + game_id + "'");
    }

    std::ostringstream request;
    request << "VALIDATE ";
    json request_data = InitSettings{settings, occupied_positions};
    request << request_data;

    json response_json =
        json::parse(invoke_logic(game_info.value(), request.str()));
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

    std::ostringstream request;
    request << "INIT ";
    json request_data = InitSettings{settings, occupied_positions};
    request << request_data;

    json response_json =
        json::parse(invoke_logic(game_info.value(), request.str()));
    auto result = response_json.get<GameStateStorage::GameState>();

    return result;
}

GameStateStorage::GameState GameLogicManager::send_move(
    const std::string &game_id,
    const GameLogicManager::GameMove &move) {
    std::optional<Game> game_info = m_games_storage->get_game_by_id(game_id);
    if (!game_info.has_value()) {
        return {};
    }

    std::ostringstream request;
    request << "MOVE ";
    json request_data = move;
    request << request_data;

    json response_json =
        json::parse(invoke_logic(game_info.value(), request.str()));
    auto result = response_json.get<GameStateStorage::GameState>();

    return result;
}

validation_error::validation_error(std::string message)
    : cavoke_base_exception(std::move(message),
                            InvalidConfig,
                            "cavoke/validation") {
}

}  // namespace cavoke::server::model