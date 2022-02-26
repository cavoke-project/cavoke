#include "participation_storage.h"

cavoke::server::model::ParticipationStorage::ParticipationStorage() {
}

std::optional<int>
cavoke::server::model::ParticipationStorage::get_participant_id(
    const std::string &session_id,
    const std::string &user_id) {
    // TODO

    if (user_id == "0") {
        return 0;
    }

    if (user_id == "1") {
        return 1;
    }

    return {};
}
