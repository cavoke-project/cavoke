#include "game.h"
#include <boost/filesystem.hpp>
#include <string>

namespace cavoke::server::model {

Game::Game(const boost::filesystem::path &directory,
           const GamesStorageConfig &game_storage_config)
    : directory(directory),
      logic_file(directory / LOGIC_FILE),
      client_file(directory / CLIENT_FILE),
      CLIENT_FILE(game_storage_config.zip_name),
      LOGIC_FILE(game_storage_config.logic_name),
      CONFIG_FILE(game_storage_config.config_name) {
    json j = json::parse(std::ifstream(directory / CONFIG_FILE));
    config = j.get<GameConfig>();
}

/// Checks whether given path is a directory
bool check_is_directory(const boost::filesystem::path &path) {
    return exists(path) && is_directory(path);
}
/// Checks whether given path exists and is a regular file
bool check_is_regular_file(const boost::filesystem::path &path) {
    return exists(path) && is_regular_file(path);
}
/// Checks whether given file is a valid `GameConfig`
bool check_valid_game_config(const boost::filesystem::path &path) {
    try {  // slow?
        json j = json::parse(std::ifstream(path));
        auto conf = j.get<GameConfig>();
        conf.validate();
        return true;
    } catch (...) {
        return false;
    }
}

bool Game::is_game_directory(const boost::filesystem::path &path,
                             const GamesStorageConfig &games_storage_config) {
    return check_is_directory(path) &&
           check_is_regular_file(path / games_storage_config.zip_name) &&
           check_is_regular_file(path / games_storage_config.logic_name) &&
           check_valid_game_config(path / games_storage_config.config_name);
}

}  // namespace cavoke::server::model
