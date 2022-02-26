#ifndef CAVOKE_CAVOKE_BASE_EXCEPTION_H
#define CAVOKE_CAVOKE_BASE_EXCEPTION_H

#include <stdexcept>

namespace cavoke::server {
/// Base exception for errors thrown in server
struct cavoke_base_exception : std::runtime_error {
public:
    explicit cavoke_base_exception(std::string message,
                                   std::string tag = "cavoke");
};
}  // namespace cavoke::server

#endif  // CAVOKE_CAVOKE_BASE_EXCEPTION_H
