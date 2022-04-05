#include <drogon/orm/Mapper.h>
#include "health_controller.h"
#include "Users.h"

void cavoke::server::controllers::HealthController::health(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto resp = drogon::HttpResponse::newHttpResponse();
    resp->setContentTypeCode(drogon::CT_TEXT_PLAIN);
    
// test code
    const auto& dbclient = drogon::app().getDbClient();
    drogon::orm::Mapper<drogon_model::cavoke_test::Users> mp(dbclient);
    std::cout << mp.count() << std::endl;
    drogon_model::cavoke_test::Users u;
    u.setId("cc3a3643-745c-40bd-b4d5-bc8201072ee9");
    mp.insert(u);
    std::cout << mp.count() << std::endl;
    
    resp->setBody("OK");
    callback(resp);
}
