#ifndef CAVOKE_SERVER_PARTICIPATION_STORAGE_H
#define CAVOKE_SERVER_PARTICIPATION_STORAGE_H

#include <optional>
#include <string>
namespace cavoke::server::model {

class ParticipationStorage {
    // TODO: thread-safety

public:
    ParticipationStorage();

    std::optional<int> get_participant_id(const std::string &session_id,
                                          const std::string &user_id);
};

}  // namespace cavoke::server::model

#endif  // CAVOKE_SERVER_PARTICIPATION_STORAGE_H
