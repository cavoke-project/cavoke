#include "game_logic_manager.h"
#include <boost/process.hpp>
#include <utility>

namespace cavoke::server::model {

GameLogicManager::GameLogicManager(std::shared_ptr<GamesStorage> games_storage)
    : m_games_storage(std::move(games_storage)) {
}

GameStateStorage::GameState GameLogicManager::send_move(
    const std::string &game_id,
    const GameLogicManager::GameMove &move) {
    std::optional<Game> game_info = m_games_storage->get_game_by_id(game_id);
    if (!game_info.has_value()) {
        return {};
    }

    // TODO: error-handling
    // TODO: move to nlohmann/json
    // TODO: logging
    std::string json_move = move.to_json().toStyledString();

    std::cout << "SEND MOVE: '" << json_move << "'" << std::endl;
    std::string json_result = invoke_logic(game_info.value(), json_move);

    GameStateStorage::GameState result =
        GameStateStorage::parse_state(json_result);
    std::cout << "GET RESULT: '" << json_result << "'" << std::endl;

    return result;
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

Json::Value GameLogicManager::GameMove::to_json() const {
    Json::Value result;
    result["player_id"] = player_id;
    result["move"] = move;
    result["global_state"] = global_state;

    return result;
}
}  // namespace cavoke::server::model
