#pragma once

#include <drogon/HttpAppFramework.h>
#include <drogon/HttpFilter.h>
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>
#include "sql-models/Users.h"
#include "utils.h"

using namespace drogon;
using nlohmann_verifier =
    jwt::verifier<jwt::default_clock, jwt::traits::nlohmann_json>;
/** Provides an authentication interceptor that checks a bearer token
 *  using a public key given in the configuration file.
 *
 *  If authenticated, passes `user_id` through via a query with the same name
 *  for controllers reverse compatibility and usage without authentication.
 *
 *  If no configuration for `auth` is found, allows all requests and acquires
 * `user_id` from query param with the same name. A corresponding warning will
 * be displayed in the logs.
 */
class AuthFilter : public HttpFilter<AuthFilter> {
public:
    AuthFilter();
    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&fcb,
                  FilterChainCallback &&fccb) override;

    struct AuthConfig {
        std::string type;
        std::string algorithm;
        std::string audience;
        std::string issuer;
        std::string public_key;
    };
    /// Acquires `user_id` parsed during `AuthFilter`'s execution
    static std::string get_user_id(const HttpRequestPtr &);

private:
    std::optional<nlohmann_verifier> verifier;

    static std::string extract_token_from_header(
        const std::string &auth_header);

    mutable MAPPER_TYPE(drogon_model::cavoke_orm::Users) mp_users =
        MAPPER_FOR(drogon_model::cavoke_orm::Users);

    void register_user(const std::string &) const;

    static const std::string AUTHORIZATION_HEADER;
    static const std::string TOKEN_PREFIX;
    static const std::string SETTINGS_KEYNAME;
    static const std::string USER_ID_QUERY_NAME;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AuthFilter::AuthConfig,
                                   type,
                                   algorithm,
                                   audience,
                                   issuer,
                                   public_key)