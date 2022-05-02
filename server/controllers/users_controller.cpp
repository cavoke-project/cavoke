#include "users_controller.h"
#include "filters/AuthFilter.h"

namespace cavoke::server::controllers {
void UsersController::get_me(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto user_id = AuthFilter::get_user_id(req);
    auto user = mp_users.findByPrimaryKey(user_id);
    auto resp = newNlohmannJsonResponse(nlohmann::to_nlohmann(user.toJson()));
    return callback(resp);
}
void UsersController::change_name(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto user_id = AuthFilter::get_user_id(req);
    
    auto new_name = req->getOptionalParameter<std::string>("new_name");
    if (!new_name.has_value()) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }
    
    mp_users.updateBy({drogon_model::cavoke_orm::Users::Cols::_display_name}, drogon::orm::Criteria(drogon_model::cavoke_orm::Users::Cols::_id, drogon::orm::CompareOperator::EQ, user_id), new_name.value());
    auto user = mp_users.findByPrimaryKey(user_id);
    auto resp = newNlohmannJsonResponse(nlohmann::to_nlohmann(user.toJson()));
    return callback(resp);
}
void UsersController::get_user(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto req_user_id = req->getOptionalParameter<std::string>("req_user_id");
    if (!req_user_id.has_value()) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }
    
    try {
        auto user = mp_users.findByPrimaryKey(req_user_id.value());
        auto resp = newNlohmannJsonResponse(nlohmann::to_nlohmann(user.toJson()));
        return callback(resp);
    } catch (const drogon::orm::UnexpectedRows&) {
        return CALLBACK_STATUS_CODE(k404NotFound);
    }
}
}