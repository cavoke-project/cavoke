#ifndef CAVOKE_ROOMS_CONTROLLER_H
#define CAVOKE_ROOMS_CONTROLLER_H

#include <drogon/HttpController.h>
#include "model/rooms/rooms_storage.h"

namespace cavoke::server::controllers {

class RoomsController : public drogon::HttpController<RoomsController, false> {
    std::shared_ptr<model::RoomsStorage> m_rooms_storage;
    std::shared_ptr<model::GamesStorage> m_games_storage;

public:
    RoomsController(std::shared_ptr<model::RoomsStorage> mRoomsStorage,
                    std::shared_ptr<model::GamesStorage> mGamesStorage);

    METHOD_LIST_BEGIN
    ADD_METHOD_TO(RoomsController::create_room,
                  "/rooms/create",
                  drogon::Post,
                  "AuthFilter");
    ADD_METHOD_TO(RoomsController::get_info,
                  "/rooms/{room_id}/get_info",
                  drogon::Get,
                  "AuthFilter");
    ADD_METHOD_TO(RoomsController::get_info_by_invite_code,
                  "/rooms/get_info_by_invite_code",
                  drogon::Get,
                  "AuthFilter");
    ADD_METHOD_TO(RoomsController::join,
                  "/rooms/join",
                  drogon::Post,
                  "AuthFilter");
    ADD_METHOD_TO(RoomsController::leave,
                  "/rooms/{room_id}/leave",
                  drogon::Post,
                  "AuthFilter");
    ADD_METHOD_TO(RoomsController::create_session,
                  "/rooms/{room_id}/create_session",
                  drogon::Post,
                  "AuthFilter");
    METHOD_LIST_END

protected:
    void create_room(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);

    void create_session(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback,
        const std::string &room_id);

    void get_info(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback,
        const std::string &room_id);

    void get_info_by_invite_code(
        const drogon::HttpRequestPtr &req,
        std::function<void(const drogon::HttpResponsePtr &)> &&callback);

    void join(const drogon::HttpRequestPtr &req,
              std::function<void(const drogon::HttpResponsePtr &)> &&callback);

    void leave(const drogon::HttpRequestPtr &req,
               std::function<void(const drogon::HttpResponsePtr &)> &&callback,
               const std::string &room_id);
};

}  // namespace cavoke::server::controllers

#endif  // CAVOKE_ROOMS_CONTROLLER_H
