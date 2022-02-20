#ifndef CAVOKE_UTILS_H
#define CAVOKE_UTILS_H

#include <drogon/HttpResponse.h>
#include <boost/filesystem/path.hpp>
#include <nlohmann/json.hpp>

namespace nlohmann {
/// nlohmann::json serializer for `boost::filesystem::path`
template <>
struct adl_serializer<boost::filesystem::path> {
    static void to_json(json &j, const boost::filesystem::path &value) {
        j = json{value.string()};
    }

    static void from_json(const json &j, boost::filesystem::path &value) {
        value = j.get<std::string>();
    }
};
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
}  // namespace cavoke::server::controllers

#endif  // CAVOKE_UTILS_H
