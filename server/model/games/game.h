#ifndef CAVOKE_SERVER_GAME_H
#define CAVOKE_SERVER_GAME_H
#include <drogon/drogon.h>
#include <boost/filesystem/path.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include "games_storage_config.h"

namespace cavoke::server::model {

struct GameConfig {
    std::string id;
    std::string display_name;
    std::string description;
    std::string app_type;
    std::string url;
    int players_num;
    std::vector<std::string> role_names;
    json default_settings;

    /// Validates config. Throws an exception if invalid
    void validate() const {
        // TODO: just a sample, `assert` removed in Release
        assert(players_num >= 1);
    }
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameConfig,
                                   id,
                                   display_name,
                                   description,
                                   app_type,
                                   url,
                                   players_num,
                                   role_names,
                                   default_settings)

class Game {
    const std::string CONFIG_FILE;
    const std::string CLIENT_FILE;
    const std::string LOGIC_FILE;

public:
    explicit Game(const boost::filesystem::path &directory,
                  const GamesStorageConfig &game_storage_config);
    Game() = default;

    boost::filesystem::path directory;
    boost::filesystem::path client_file;
    GameConfig config;
    boost::filesystem::path logic_file;

    static bool is_game_directory(
        const boost::filesystem::path &path,
        const GamesStorageConfig &games_storage_config);
};

}  // namespace cavoke::server::model

#endif  // CAVOKE_SERVER_GAME_H
