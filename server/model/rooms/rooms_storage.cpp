#include "rooms_storage.h"
#include <utility>
#include "drogon/orm/Criteria.h"
#include "sql-models/RoomJoins.h"

namespace cavoke::server::model {

using namespace drogon::orm;

RoomsStorage::RoomsStorage(std::shared_ptr<SessionsStorage> mSessionsStorage)
    : m_sessions_storage(std::move(mSessionsStorage)) {
}

/// Generates an invite code for session
std::string RoomsStorage::generate_invite_code() {
    // prepare template
    std::string res = "......";
    // random digits
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<char> dist('0', '9');
    // set every character to be a random digit
    std::generate(res.begin(), res.end(),
                  [&dist, &engine]() { return dist(engine); });
    return res;
}

std::optional<RoomsStorage::RoomInfo> RoomsStorage::get_by_id(
    const std::string &room_id) {
    auto mp_rooms_trans = MAPPER_FOR(drogon_model::cavoke_orm::Rooms);
    try {
        const auto &room = mp_rooms_trans.findByPrimaryKey(room_id);
        const auto &members = get_members(room_id);
        return RoomInfo::from_room_and_members(room, members);
    } catch (const UnexpectedRows &e) {
        return std::nullopt;
    }
}

std::vector<GameSessionAccessObject::UserInfo> RoomsStorage::get_members(
    const std::string &room_id) {
    auto mp_rooms_trans = MAPPER_FOR(drogon_model::cavoke_orm::Rooms);
    auto mp_joins_trans = MAPPER_FOR(drogon_model::cavoke_orm::RoomJoins);
    auto mp_users_trans = MAPPER_FOR(
        drogon_model::cavoke_orm::Users);  // !!! we need a users controller !!!

    auto joins = mp_joins_trans.findBy(
        Criteria(drogon_model::cavoke_orm::RoomJoins::Cols::_room_id,
                 CompareOperator::EQ, room_id));

    std::vector<GameSessionAccessObject::UserInfo> members;
    members.reserve(joins.size());
    std::transform(
        joins.begin(), joins.end(), std::back_inserter(members),
        [&mp_users_trans](const drogon_model::cavoke_orm::RoomJoins &join) {
            return GameSessionAccessObject::UserInfo::from_user(
                mp_users_trans.findByPrimaryKey(join.getValueOfUserId()));
        });

    return members;
}

std::optional<RoomsStorage::RoomInfo> RoomsStorage::get_by_invite_code(
    const std::string &invite_code) {
    auto transaction = drogon::app().getDbClient()->newTransaction();
    auto mp_rooms_trans = MAPPER_FOR(drogon_model::cavoke_orm::Rooms);
    try {
        const auto &room = mp_rooms_trans.findOne(
            Criteria(drogon_model::cavoke_orm::Rooms::Cols::_invite_code,
                     CompareOperator::EQ, invite_code));
        const auto &members = get_members(room.getValueOfId());
        return RoomInfo::from_room_and_members(room, members);

    } catch (const UnexpectedRows &) {
        return std::nullopt;
    }
}

void RoomsStorage::add_user(const std::string &room_id,
                            const std::string &user_id) {
    if (is_member(room_id, user_id)) {
        return;
    }

    if (!get_by_id(room_id).has_value()) {
        throw room_error("Cannot add user. No room with id " + room_id);
    }

    auto join = drogon_model::cavoke_orm::RoomJoins();
    {
        join.setRoomId(room_id);
        join.setUserId(user_id);
    }

    auto mp_joins_trans = MAPPER_FOR(drogon_model::cavoke_orm::RoomJoins);
    mp_joins_trans.insert(join);
}

void RoomsStorage::remove_user(const std::string &room_id,
                               const std::string &user_id) {
    if (!is_member(room_id, user_id)) {
        return;
    }
    drogon::app().getDbClient()->execSqlSync(
        "select leave_room($1::uuid, $2::varchar);", room_id, user_id);
}

bool RoomsStorage::is_member(const std::string &room_id,
                             const std::string &user_id) {
    auto mp_joins_trans = MAPPER_FOR(drogon_model::cavoke_orm::RoomJoins);
    size_t joins_count = mp_joins_trans.count(
        Criteria(drogon_model::cavoke_orm::RoomJoins::Cols::_room_id,
                 CompareOperator::EQ, room_id) &&
        Criteria(drogon_model::cavoke_orm::RoomJoins::Cols::_user_id,
                 CompareOperator::EQ, user_id));
    return joins_count > 0;
}

RoomsStorage::RoomInfo RoomsStorage::create_room(
    const std::string &host_id,
    const std::string &display_name) {
    auto room = drogon_model::cavoke_orm::Rooms();
    {
        room.setId(drogon::utils::getUuid());
        // TODO: there are only 1e6 invite codes, something has to be done about
        room.setInviteCode(generate_invite_code());
        room.setDisplayName(display_name);
        room.setSessionIdToNull();
    }

    auto join = drogon_model::cavoke_orm::RoomJoins();
    {
        join.setRoomId(room.getValueOfId());
        join.setUserId(host_id);
    }

    {
        //        auto transaction =
        //        drogon::app().getDbClient()->newTransaction();
        auto mp_rooms_trans = MAPPER_FOR(drogon_model::cavoke_orm::Rooms);
        auto mp_joins_trans = MAPPER_FOR(drogon_model::cavoke_orm::RoomJoins);

        mp_rooms_trans.insert(room);
        mp_joins_trans.insert(join);

        room.setHostId(host_id);
        mp_rooms_trans.update(room);
    }

    return RoomsStorage::RoomInfo::from_room_and_members(
        room, get_members(room.getValueOfId()));
}

GameSessionAccessObject::GameSessionInfo RoomsStorage::create_session(
    const std::string &room_id,
    const GameConfig &game_config) {
    auto opt_room_info = get_by_id(room_id);
    if (!opt_room_info.has_value()) {
        throw room_error("Cannot create session. No room with id " + room_id);
    }
    auto room_info = opt_room_info.value();

    if (!room_info.session_id.empty()) {
        auto current_session =
            m_sessions_storage->get_sessionAO(room_info.session_id);
        if (current_session.get_session_info().status !=
            GameSessionAccessObject::SessionStatus::FINISHED) {
            throw room_error("Cannot create session in room " + room_id +
                             ". Session already exists and not "
                             "finished.");
        } else {
            current_session.delete_session();
        }
    }

    auto new_session = m_sessions_storage->create_session(
        game_config, room_info.host_id, room_id);

    return new_session;
}

room_error::room_error(std::string message)
    : cavoke_base_exception(std::move(message),
                            InvalidClientInput,
                            "cavoke/rooms") {
}

RoomsStorage::RoomInfo RoomsStorage::RoomInfo::from_room_and_members(
    const drogon_model::cavoke::Rooms &room_orm,
    const std::vector<GameSessionAccessObject::UserInfo> &members) {
    return {room_orm.getValueOfId(),          room_orm.getValueOfInviteCode(),
            room_orm.getValueOfDisplayName(), room_orm.getValueOfSessionId(),
            room_orm.getValueOfHostId(),      members};
}
}  // namespace cavoke::server::model
