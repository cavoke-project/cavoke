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
  ADD_METHOD_TO(GamesController::game_config, "/games/{game_id}/get_config",
                drogon::Get);
  ADD_METHOD_TO(GamesController::game_client_file,
                "/games/{game_id}/get_client", drogon::Get);
  METHOD_LIST_END

  explicit GamesController(std::shared_ptr<model::GamesStorage> games_storage);

protected:
  void
  games_list(const drogon::HttpRequestPtr &req,
             std::function<void(const drogon::HttpResponsePtr &)> &&callback);

  void
  game_config(const drogon::HttpRequestPtr &req,
              std::function<void(const drogon::HttpResponsePtr &)> &&callback,
              const std::string& game_id);

  void game_client_file(
      const drogon::HttpRequestPtr &req,
      std::function<void(const drogon::HttpResponsePtr &)> &&callback,
      const std::string& game_id);
};

} // namespace cavoke::server::controllers
#endif // CAVOKE_SERVER_GAMES_CONTROLLER_H
