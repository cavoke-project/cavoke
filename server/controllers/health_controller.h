#ifndef CAVOKE_SERVER_HEALTH_CONTROLLER_H
#define CAVOKE_SERVER_HEALTH_CONTROLLER_H

#include <drogon/HttpController.h>

namespace cavoke::server::controllers {

class HealthController : public drogon::HttpController<HealthController, true> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(HealthController::health, "/health", drogon::Get);
    METHOD_LIST_END

protected:
    void health(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};

}  // namespace cavoke::server::controllers

#endif  // CAVOKE_SERVER_HEALTH_CONTROLLER_H
