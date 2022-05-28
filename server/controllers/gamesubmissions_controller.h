#ifndef CAVOKE_GAMESUBMISSIONS_CONTROLLER_H
#define CAVOKE_GAMESUBMISSIONS_CONTROLLER_H

#include <drogon/HttpController.h>
#include <nlohmann/json.hpp>
#include "sql-models/Gamesubmissions.h"
#include "utils.h"

namespace cavoke::server::controllers {

class GameSubmissionsController
    : public drogon::HttpController<GameSubmissionsController, true> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(GameSubmissionsController::submit_game,
                  "/submit_game",
                  drogon::Post,
                  drogon::Options);
    METHOD_LIST_END

    struct GameSubmissionReq {
        std::string game_id;
        std::string display_name;
        std::string description;
        std::string package_type;
        std::string git_repo;

        auto to_orm(const std::string &id) {
            drogon_model::cavoke_orm::Gamesubmissions res;
            res.setId(id);
            res.setGameId(game_id);
            res.setPackageType(package_type);
            res.setGitRepo(git_repo);
            res.setDisplayName(display_name);
            return res;
        }
    };

protected:
    void submit_game(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);

private:
    MAPPER_TYPE(drogon_model::cavoke_orm::Gamesubmissions)
    mp_submissions = MAPPER_FOR(drogon_model::cavoke_orm::Gamesubmissions);
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameSubmissionsController::GameSubmissionReq,
                                   game_id,
                                   display_name,
                                   description,
                                   package_type,
                                   git_repo);

}  // namespace cavoke::server::controllers

#endif  // CAVOKE_GAMESUBMISSIONS_CONTROLLER_H
