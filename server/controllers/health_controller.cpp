#include <drogon/orm/Mapper.h>
#include "health_controller.h"

void cavoke::server::controllers::HealthController::health(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setContentTypeCode(drogon::CT_TEXT_PLAIN);
    resp->setBody("OK");
    callback(resp);
}
