#ifndef CAVOKE_SERVER_GAMES_CONTROLLER_H
#define CAVOKE_SERVER_GAMES_CONTROLLER_H

#include "../model/games_storage.h"
#include <drogon/HttpController.h>

namespace cavoke::server::controllers {

class GamesController : public drogon::HttpController<GamesController, false> {
  std::shared_ptr<model::GamesStorage> m_games_storage;

public:
  METHOD_LIST_BEGIN
  ADD_METHOD_TO(GamesController::games_list, "/games/list", drogon::Get);
  METHOD_LIST_END

  explicit GamesController(std::shared_ptr<model::GamesStorage> games_storage);

protected:
  void
  games_list(const drogon::HttpRequestPtr &req,
             std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};

} // namespace cavoke::server::controllers
#endif // CAVOKE_SERVER_GAMES_CONTROLLER_H
