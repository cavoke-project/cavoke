#include "controllers/games_controller.h"
#include "controllers/state_controller.h"
#include "model/game_logic_manager.h"
#include "model/games_storage.h"
#include "model/participation_storage.h"
#include <drogon/HttpAppFramework.h>

namespace cavoke::server {
void run() {
  // TODO: logging

  // init models
  std::cout << "Initialize models..." << std::endl;
  auto games_storage = std::make_shared<model::GamesStorage>(
      model::GamesStorage::GamesStorageConfig{"../../local_server/games"});
  auto game_logic_manager =
      std::make_shared<model::GameLogicManager>(games_storage);
  auto game_state_storage = std::make_shared<model::GameStateStorage>();
  auto participation_storage = std::make_shared<model::ParticipationStorage>();

  // init controllers
  std::cout << "Initialize controllers..." << std::endl;
  auto games_controller =
      std::make_shared<controllers::GamesController>(games_storage);
  auto state_controller = std::make_shared<controllers::StateController>(
      games_storage, game_logic_manager, game_state_storage,
      participation_storage);

  // register controllers
  drogon::app().registerController(games_controller);
  drogon::app().registerController(state_controller);

  // RUN!
  std::cout << "Run server on 127.0.0.1:8080" << std::endl;
  drogon::app().addListener("127.0.0.1", 8080).run();
}
} // namespace cavoke::server

int main() {
  cavoke::server::run();
  return 0;
}
