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
        // if no authentication configured, just get user id parameter
        if (!verifier.has_value()) {
            auto user_id =
                req->getOptionalParameter<std::string>(USER_ID_QUERY_NAME);
            if (!user_id.has_value()) {
                throw std::runtime_error("no user_id query parameter");
            }
            register_user(user_id.value());
            return fccb();
        }
        // get authorization header
        std::string token =
            extract_token_from_header(req->getHeader(AUTHORIZATION_HEADER));
        // decode and verify Bearer token
        auto decoded = jwt::decode<jwt::traits::nlohmann_json>(token);
        verifier->verify(decoded);
        // Pass user id to methods
        auto subject = decoded.get_subject();
        req->setParameter(USER_ID_QUERY_NAME, subject);
        return fccb();
    } catch (const std::exception &) {
        // Authentication failed
        return fcb(cavoke::server::controllers::newStatusCodeResponse(
            k401Unauthorized));
    }
}

std::string AuthFilter::extract_token_from_header(
    const std::string &auth_header) {
    if (auth_header.size() < TOKEN_PREFIX.size()) {
        throw std::invalid_argument("Invalid Bearer token");
    }
    return {auth_header.begin() + TOKEN_PREFIX.size(), auth_header.end()};
}

void AuthFilter::register_user(const std::string &user_id) const {
    drogon_model::cavoke_orm::Users user;
    user.setId(user_id);
    try {
        mp_users.insert(user);
    } catch (...) {
    }
}

std::string AuthFilter::get_user_id(const HttpRequestPtr &req) {
    auto user_id = req->getOptionalParameter<std::string>(USER_ID_QUERY_NAME);
    if (!user_id.has_value()) {
        throw std::logic_error(
            "no user_id query in request, but passed auth?!");
    }
    return user_id.value();
}

const std::string AuthFilter::AUTHORIZATION_HEADER = "Authorization";
const std::string AuthFilter::TOKEN_PREFIX = "Bearer ";
const std::string AuthFilter::SETTINGS_KEYNAME = "auth";
const std::string AuthFilter::USER_ID_QUERY_NAME = "user_id";
