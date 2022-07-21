#ifndef CAVOKE_UTILS_H
#define CAVOKE_UTILS_H

#include <drogon/HttpResponse.h>
#include <drogon/utils/Utilities.h>
#include <json/json.h>
#include <boost/filesystem/path.hpp>
#include <nlohmann/json.hpp>
#include <regex>
#include "cavoke_base_exception.h"

namespace nlohmann {
/// nlohmann::json serializer for `boost::filesystem::path`
template <>
struct adl_serializer<boost::filesystem::path> {
    static void to_json(json &j, const boost::filesystem::path &value) {
        j = value.string();
    }

    static void from_json(const json &j, boost::filesystem::path &value) {
        value = j.get<std::string>();
    }
};

inline nlohmann::json to_nlohmann(const Json::Value &obj) {
    static Json::StreamWriterBuilder builder;
    return json::parse(Json::writeString(builder, obj), nullptr, true, true);
}
}  // namespace nlohmann

namespace cavoke::server::controllers {

/**
 * Creates a drogon response from serializable object.
 *
 * Preferably pass the object itself, not the json object,
 * because `nlohmann::json{configs}` adds extra square brackets to objects
 * see https://github.com/cavoke-project/cavoke/issues/22. Avoid curly braces.
 */
inline drogon::HttpResponsePtr newNlohmannJsonResponse(
    const nlohmann::json &obj) {
    auto res = drogon::HttpResponse::newHttpResponse();
    res->setContentTypeCode(drogon::CT_APPLICATION_JSON);
    res->setBody(obj.dump());
    return res;
}

/// Creates blank http response with given status code
inline drogon::HttpResponsePtr newStatusCodeResponse(
    const drogon::HttpStatusCode &status_code) {
    auto res = drogon::HttpResponse::newHttpResponse();
    res->setStatusCode(status_code);
    return res;
}

inline drogon::HttpResponsePtr newCavokeErrorResponse(
    const cavoke_base_exception &err,
    const drogon::HttpStatusCode &status_code = drogon::k200OK) {
    auto resp = newNlohmannJsonResponse(err);
    resp->setStatusCode(status_code);
    return resp;
}

#define CALLBACK_STATUS_CODE(code) \
    callback(newStatusCodeResponse(::drogon::code))
}  // namespace cavoke::server::controllers

namespace cavoke::server {
/// Hides password from sql connection info
inline std::string hide_password(std::string str) {
    return std::regex_replace(str, std::regex(R"(\b[^\s]*pass[^\s]*\b)"),
                              R"(<pass hidden>)");
}
}  // namespace cavoke::server

#define MAPPER_TYPE(T) ::drogon::orm::Mapper<T>
/// ORM mapper for given database client (e.g. transaction dbclient)
#define MAPPER_WITH_CLIENT_FOR(T, dbclient) MAPPER_TYPE(T)(dbclient)
/// ORM mapper for default database client
#define MAPPER_FOR(T) MAPPER_WITH_CLIENT_FOR(T, drogon::app().getDbClient())

/// Returns random uuid in format like `f3708899-d932-45e3-a4f8-6f43e7af8d1b`
inline std::string getLowerUUID() {
    std::string res = drogon::utils::getUuid();
    for (char &c : res) {
        // All characters to lower
        c = tolower(c);
    }
    return res;
}

#endif  // CAVOKE_UTILS_H
