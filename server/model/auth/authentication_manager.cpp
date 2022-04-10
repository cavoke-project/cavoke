#include "authentication_manager.h"
#include <drogon/HttpAppFramework.h>
#include "sql-models/Users.h"
#include "utils.h"
namespace cavoke::server::model {
using namespace drogon::orm;
using namespace drogon_model::cavoke_test;
bool cavoke::server::model::AuthenticationManager::verify_authentication(
    const std::string &user_id) const {
    // TODO auth
    auto mp_users = MAPPER_FOR(Users);
    Users user;
    user.setId(user_id);
    if (0 == mp_users.count(drogon::orm::Criteria(
                 Users::Cols::_id, CompareOperator::EQ, user_id))) {
        mp_users.insert(
            user,
            [user_id](const drogon_model::cavoke_test::Users &) {
                LOG_DEBUG << "Added: " << user_id;
            },
            [user_id](const drogon::orm::DrogonDbException &) {
                LOG_DEBUG << "Skipped: " << user_id;
            });
    }
    return true;
}
}  // namespace cavoke::server::model