#ifndef CAVOKE_USERS_CONTROLLER_H
#define CAVOKE_USERS_CONTROLLER_H
#include <drogon/HttpController.h>
#include "sql-models/Users.h"
#include "utils.h"

namespace cavoke::server::controllers {

struct UsersController : public drogon::HttpController<UsersController, true> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(UsersController::get_me, "/profile/get_me", drogon::Get, "AuthFilter");
    ADD_METHOD_TO(UsersController::change_name, "/profile/change_name", drogon::Post, "AuthFilter");
    ADD_METHOD_TO(UsersController::get_user, "/users/get_user", drogon::Get, "AuthFilter");
    METHOD_LIST_END
    
protected:
    void get_me(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void change_name(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    void get_user(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);
private:
    MAPPER_TYPE(drogon_model::cavoke_orm::Users) mp_users = MAPPER_FOR(drogon_model::cavoke_orm::Users);
};

}

#endif  // CAVOKE_USERS_CONTROLLER_H
