#ifndef CAVOKE_GAMESSTORAGECONFIG_H
#define CAVOKE_GAMESSTORAGECONFIG_H

#include <boost/filesystem/path.hpp>
#include <nlohmann/json.hpp>
#include "../utils.h"  // TODO: avoid relative paths
namespace cavoke::server::model {
using json = nlohmann::json;

struct GamesStorageConfig {
    boost::filesystem::path games_directory;
    std::string logic_name;
    std::string zip_name;
    std::string config_name;

    static GamesStorageConfig load(const std::string &config_file);
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GamesStorageConfig,
                                   games_directory,
                                   logic_name,
                                   zip_name,
                                   config_name)
}  // namespace cavoke::server::model
#endif  // CAVOKE_GAMESSTORAGECONFIG_H
