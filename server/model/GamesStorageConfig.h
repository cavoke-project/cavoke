#ifndef CAVOKE_GAMESSTORAGECONFIG_H
#define CAVOKE_GAMESSTORAGECONFIG_H

#include <boost/filesystem/path.hpp>
namespace cavoke::server::model {
struct GamesStorageConfig {
    const boost::filesystem::path games_directory;
    const std::string logic_name;
    const std::string zip_name;
    const std::string json_name;
    static GamesStorageConfig load(const std::string &config_file);
};
}  // namespace cavoke::server::model
#endif  // CAVOKE_GAMESSTORAGECONFIG_H
