#include "cavoke_base_exception.h"
cavoke::server::cavoke_base_exception::cavoke_base_exception(
    std::string message,
    std::string tag)
    : std::runtime_error("[" + std::move(tag) + "]: " + std::move(message)) {
}
