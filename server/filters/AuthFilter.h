#pragma once

#include <drogon/HttpAppFramework.h>
#include <drogon/HttpFilter.h>
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>
#include "utils.h"

using namespace drogon;
using nlohmann_verifier =
    jwt::verifier<jwt::default_clock, jwt::traits::nlohmann_json>;
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

private:
    std::optional<nlohmann_verifier> verifier;

    static std::string extract_token_from_header(
        const std::string &auth_header);
    static const std::string AUTHORIZATION_HEADER;
    static const std::string TOKEN_PREFIX;
    static const std::string SETTINGS_KEYNAME;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AuthFilter::AuthConfig,
                                   type,
                                   algorithm,
                                   audience,
                                   issuer,
                                   public_key)