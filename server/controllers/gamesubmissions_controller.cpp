#include "gamesubmissions_controller.h"
#include "filters/AuthFilter.h"

namespace cavoke::server::controllers {

void GameSubmissionsController::submit_game(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    // TODO: Auth. Currently problems with authenticating the form request.
    // parse web form
    drogon::MultiPartParser mpp;
    int res = mpp.parse(req);
    if (res == -1) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }
    // form files
    auto files = mpp.getFilesMap();
    // form params
    nlohmann::json j = mpp.getParameters();
    if (!j.contains("git_repo")) {
        j["git_repo"] = "<empty>";
    }
    // serialize
    GameSubmissionReq req_data;
    try {
        req_data = j.get<GameSubmissionReq>();
    } catch (...) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }
    // validation
    if (req_data.package_type != "Git Repository" &&
        req_data.package_type != "Zip Archive") {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }

    auto id = drogon::utils::getUuid();
    auto submission = req_data.to_orm(id);
    if (req_data.package_type == "Zip Archive") {
        try {
            files.at("client_zip").saveAs(id + "-client.zip");
            files.at("server_zip").saveAs(id + "-server.zip");
        } catch (...) {
            return CALLBACK_STATUS_CODE(k400BadRequest);
        }
    }
    // save to db
    mp_submissions.insert(submission);

    LOG_INFO << "New game submission! Game: " << req_data.display_name;
    auto resp = newStatusCodeResponse(drogon::k200OK);
    resp->setBody(
        "Thanks for your game submission! We will review it and publish your "
        "game shortly.");
    return callback(resp);
}
}  // namespace cavoke::server::controllers