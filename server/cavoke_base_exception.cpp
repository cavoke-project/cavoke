#include "cavoke_base_exception.h"
cavoke::server::cavoke_base_exception::cavoke_base_exception(
    std::string message,
    error_code code,
    std::string tag)
    : std::runtime_error("[" + std::move(tag) + "]: " + message),
      message(std::move(message)),
      code(code) {
}
