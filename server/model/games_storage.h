#ifndef CAVOKE_GAMES_STORAGE_H
#define CAVOKE_GAMES_STORAGE_H

#include <boost/filesystem/path.hpp>
#include <drogon/drogon.h>
#include <string>
namespace cavoke::server::model {

class Game {
  static const std::string CONFIG_FILE;
  static const std::string CLIENT_FILE;
  static const std::string LOGIC_FILE;

  boost::filesystem::path directory;

  struct GameConfig {
    std::string id;
    std::string display_name;
    std::string description;
    int players_num;

    [[nodiscard]] Json::Value to_json() const;
  };

  static bool read_config_file(const boost::filesystem::path &path,
                               Json::Value &json_obj);

public:
  explicit Game(boost::filesystem::path directory);

  boost::filesystem::path client_file;
  GameConfig config;
  boost::filesystem::path logic_file;

  static bool is_game_directory(const boost::filesystem::path &path);
};

class GamesStorage {
  // TODO: thread-safety
public:
  struct GamesStorageConfig {
    boost::filesystem::path games_directory;
  };

  explicit GamesStorage(GamesStorageConfig config);

  void update();

  std::vector<Game> list_games();

private:
  GamesStorageConfig m_config;
  std::map<std::string, Game> m_games;
};

} // namespace cavoke::server::model

#endif // CAVOKE_GAMES_STORAGE_H
