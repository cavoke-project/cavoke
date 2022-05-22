#ifndef CAVOKE_ROOMS_STORAGE_H
#define CAVOKE_ROOMS_STORAGE_H

#include <memory>
#include "model/sessions/sessions_storage.h"
#include "sql-models/Rooms.h"

namespace cavoke::server::model {

/// exception for errors thrown during actions with rooms
struct room_error : cavoke_base_exception {
    explicit room_error(std::string message);
};

class RoomsStorage {
    std::shared_ptr<SessionsStorage> m_sessions_storage;

    static std::string generate_invite_code();

public:
    explicit RoomsStorage(std::shared_ptr<SessionsStorage> mSessionsStorage);

    struct RoomInfo {
        std::string room_id;
        std::string invite_code;
        std::string display_name;
        std::string session_id;  // let "" mean null
        std::string host_id;
        std::vector<GameSessionAccessObject::UserInfo> members;

        static RoomInfo from_room_and_members(
            const drogon_model::cavoke::Rooms &room_orm,
            const std::vector<GameSessionAccessObject::UserInfo> &members);
    };

    std::vector<GameSessionAccessObject::UserInfo> get_members(
        const std::string &room_id);

    std::optional<RoomInfo> get_by_id(const std::string &room_id);

    std::optional<RoomInfo> get_by_invite_code(const std::string &invite_code);

    RoomInfo create_room(const std::string &host_id,
                         const std::string &display_name);

    void add_user(const std::string &room_id, const std::string &user_id);

    void remove_user(const std::string &room_id, const std::string &user_id);

    bool is_member(const std::string &room_id, const std::string &user_id);

    GameSessionAccessObject::GameSessionInfo create_session(
        const std::string &room_id,
        const GameConfig &game_config);
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RoomsStorage::RoomInfo,
                                   room_id,
                                   invite_code,
                                   display_name,
                                   session_id,
                                   host_id,
                                   members)

}  // namespace cavoke::server::model
#endif  // CAVOKE_ROOMS_STORAGE_H
