#ifndef CAVOKE_STATE_CONTROLLER_H
#define CAVOKE_STATE_CONTROLLER_H

#include <drogon/HttpController.h>

namespace cavoke::logic_server::controllers {

class LogicController : public drogon::HttpController<LogicController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(LogicController::validate, "/validate", drogon::Post);
    ADD_METHOD_TO(LogicController::init_state, "/init_state", drogon::Post);
    ADD_METHOD_TO(LogicController::apply_move, "/apply_move", drogon::Post);
    METHOD_LIST_END

protected:
    void validate(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);

    void init_state(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);

    void apply_move(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};

}  // namespace cavoke::logic_server::controllers

#endif  // CAVOKE_STATE_CONTROLLER_H
