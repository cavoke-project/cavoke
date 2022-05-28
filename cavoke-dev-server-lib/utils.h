#ifndef CAVOKE_UTILS_H
#define CAVOKE_UTILS_H

#include <drogon/HttpResponse.h>
#include <boost/filesystem/path.hpp>
#include <nlohmann/json.hpp>
#include <regex>

namespace cavoke::logic_server::controllers {

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

#define CALLBACK_STATUS_CODE(code) \
    callback(newStatusCodeResponse(::drogon::code))
}  // namespace cavoke::logic_server::controllers

#endif  // CAVOKE_UTILS_H
