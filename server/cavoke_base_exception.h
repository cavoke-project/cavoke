#ifndef CAVOKE_CAVOKE_BASE_EXCEPTION_H
#define CAVOKE_CAVOKE_BASE_EXCEPTION_H

#include <nlohmann/json.hpp>
#include <stdexcept>

namespace cavoke::server {
/// Base exception for errors thrown in server
struct cavoke_base_exception : std::runtime_error {
public:
    enum error_code { UnknownError = 0, InvalidClientInput, NotFound, InvalidConfig };

    cavoke_base_exception(std::string message,
                          error_code code,
                          std::string tag = "cavoke");

private:
    std::string message;
    error_code code;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(cavoke_base_exception, code, message);
};

}  // namespace cavoke::server

#endif  // CAVOKE_CAVOKE_BASE_EXCEPTION_H
