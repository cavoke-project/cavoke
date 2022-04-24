#include "AuthFilter.h"
#include <stdexcept>

using namespace drogon;
AuthFilter::AuthFilter() {
    auto app_config = nlohmann::to_nlohmann(drogon::app().getCustomConfig());
    if (app_config.contains(SETTINGS_KEYNAME)) {
        AuthConfig config = app_config.at("auth").get<AuthConfig>();
        assert(config.type == "JWT");
        assert(config.algorithm == "RS256");
        verifier =
            nlohmann_verifier{jwt::default_clock()}
                .with_audience(config.audience)
                .with_issuer(config.issuer)
                .allow_algorithm(jwt::algorithm::rs256(config.public_key));
        LOG_INFO << config.type << " auth guard enabled (iss=" << config.issuer
                 << ", aud=" << config.audience << ")";
    } else {
        LOG_WARN << "No auth guard used...";
        verifier = {};
    }
}
void AuthFilter::doFilter(const HttpRequestPtr &req,
                          FilterCallback &&fcb,
                          FilterChainCallback &&fccb) {
    try {
        // if no authentication configured, skip
        if (!verifier.has_value()) {
            fccb();
            return;
        }
        // get authorization header
        std::string token =
            extract_token_from_header(req->getHeader(AUTHORIZATION_HEADER));
        // decode and verify Bearer token
        auto decoded = jwt::decode<jwt::traits::nlohmann_json>(token);
        verifier->verify(decoded);
        // Pass user id to methods
        auto subject = decoded.get_subject();
        req->setParameter("user_id", subject);
        fccb();
    } catch (const std::exception &) {
        // Authentication failed
        auto res = drogon::HttpResponse::newHttpResponse();
        res->setStatusCode(drogon::k401Unauthorized);
        fcb(res);
    }
}
std::string AuthFilter::extract_token_from_header(
    const std::string &auth_header) {
    if (auth_header.size() < TOKEN_PREFIX.size()) {
        throw std::invalid_argument("Invalid Bearer token");
    }
    return {auth_header.begin() + TOKEN_PREFIX.size(), auth_header.end()};
}

const std::string AuthFilter::AUTHORIZATION_HEADER = "Authorization";
const std::string AuthFilter::TOKEN_PREFIX = "Bearer ";
const std::string AuthFilter::SETTINGS_KEYNAME = "auth";
