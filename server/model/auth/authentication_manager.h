#ifndef CAVOKE_AUTHENTICATION_MANAGER_H
#define CAVOKE_AUTHENTICATION_MANAGER_H

#include <string>

namespace cavoke::server::model {
/// Manager for checking user's authentication.
/// In future it will be JWT/JWK verification.
class AuthenticationManager {
public:
    /// Checks whether user_id is valid. signature will change in the future.
    [[nodiscard]] bool verify_authentication(const std::string &user_id) const;
};
}  // namespace cavoke::server::model

#endif  // CAVOKE_AUTHENTICATION_MANAGER_H
