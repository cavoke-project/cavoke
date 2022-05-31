#include "rooms_controller.h"
#include <utility>
#include "filters/AuthFilter.h"

namespace cavoke::server::controllers {

void RoomsController::create_room(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    // get user id
    auto user_id = AuthFilter::get_user_id(req);

    auto display_name = req->getOptionalParameter<std::string>("display_name");
    if (!display_name.has_value()) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }

    auto room = m_rooms_storage->create_room(user_id, display_name.value());
    return callback(newNlohmannJsonResponse(room));
}

void RoomsController::create_session(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &room_id) {
    // get game id
    auto game_id = req->getOptionalParameter<std::string>("game_id");
    if (!game_id.has_value()) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }

    auto room_info = m_rooms_storage->get_by_id(room_id);
    if (!room_info.has_value()) {
        return CALLBACK_STATUS_CODE(k404NotFound);
    }

    // get user id
    auto user_id = AuthFilter::get_user_id(req);

    if (room_info.value().host_id != user_id) {
        return CALLBACK_STATUS_CODE(k403Forbidden);
    }

    auto game = m_games_storage->get_game_by_id(game_id.value());
    if (!game.has_value()) {
        return callback(newCavokeErrorResponse(
            {"game '" + game_id.value() + "' not found",
             cavoke_base_exception::error_code::NotFound},
            drogon::k404NotFound));
    }

    try {
        auto session =
            m_rooms_storage->create_session(room_id, game.value().config);
        return callback(newNlohmannJsonResponse(session));
    } catch (const model::room_error &e) {
        return callback(newCavokeErrorResponse(e, k403Forbidden));
    }
}

void RoomsController::get_info(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &room_id) {
    auto room_info = m_rooms_storage->get_by_id(room_id);
    if (!room_info.has_value()) {
        return CALLBACK_STATUS_CODE(k404NotFound);
    }

    if (!room_info->session_id.empty()) {
        room_info->session =
            m_sessions_storage->get_sessionAO(room_info->session_id)
                .get_session_info();
    }
    json json_res = room_info.value();
    if (room_info->session_id.empty()) {
        json_res.erase("session");
    }

    return callback(newNlohmannJsonResponse(json_res));
}

void RoomsController::get_info_by_invite_code(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto invite_code = req->getOptionalParameter<std::string>("invite_code");
    if (!invite_code.has_value()) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }

    auto room_info = m_rooms_storage->get_by_invite_code(invite_code.value());
    if (!room_info.has_value()) {
        return CALLBACK_STATUS_CODE(k404NotFound);
    }

    if (!room_info->session_id.empty()) {
        room_info->session =
            m_sessions_storage->get_sessionAO(room_info->session_id)
                .get_session_info();
    }
    json json_res = room_info.value();
    if (room_info->session_id.empty()) {
        json_res.erase("session");
    }

    return callback(newNlohmannJsonResponse(json_res));
}

void RoomsController::join(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto invite_code = req->getOptionalParameter<std::string>("invite_code");
    if (!invite_code.has_value()) {
        return CALLBACK_STATUS_CODE(k400BadRequest);
    }

    auto room_info = m_rooms_storage->get_by_invite_code(invite_code.value());
    if (!room_info.has_value()) {
        return CALLBACK_STATUS_CODE(k404NotFound);
    }

    // get user id
    auto user_id = AuthFilter::get_user_id(req);

    try {
        m_rooms_storage->add_user(room_info.value().room_id, user_id);
        auto result =
            m_rooms_storage->get_by_invite_code(invite_code.value()).value();
        if (!result.session_id.empty()) {
            result.session =
                m_sessions_storage->get_sessionAO(result.session_id)
                    .get_session_info();
        }
        json json_res = result;
        if (result.session_id.empty()) {
            json_res.erase("session");
        }

        return callback(newNlohmannJsonResponse(json_res));
    } catch (const model::room_error &e) {
        return callback(newCavokeErrorResponse(e, drogon::k404NotFound));
    }
}

void RoomsController::leave(
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    const std::string &room_id) {
    auto room_info = m_rooms_storage->get_by_id(room_id);
    if (!room_info.has_value()) {
        return CALLBACK_STATUS_CODE(k404NotFound);
    }

    // get user id
    auto user_id = AuthFilter::get_user_id(req);

    try {
        m_rooms_storage->remove_user(room_id, user_id);
    } catch (const model::room_error &e) {
        return callback(newCavokeErrorResponse(e, drogon::k404NotFound));
    }

    return CALLBACK_STATUS_CODE(k200OK);
}

RoomsController::RoomsController(
    std::shared_ptr<model::RoomsStorage> mRoomsStorage,
    std::shared_ptr<model::GamesStorage> mGamesStorage,
    std::shared_ptr<model::SessionsStorage> mSessionsStorage)
    : m_rooms_storage(std::move(mRoomsStorage)),
      m_games_storage(std::move(mGamesStorage)),
      m_sessions_storage(std::move(mSessionsStorage)) {
}

}  // namespace cavoke::server::controllers
