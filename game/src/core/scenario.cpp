#include "scenario.h"

#include "assets.h"
#include "config_loader.h"
#include "game.h"

namespace mengde {
namespace core {

Scenario::Scenario(const string& scenario_id) : rc_(), assets_(nullptr), game_(nullptr) {
  const Path base_path(scenario_id);
  ConfigLoader loader(base_path / "config.lua");
  rc_ = loader.GetResources();

  // For the case of NEW GAME
  assets_ = new Assets();
  game_ = NewGame(scenario_id, loader.GetFirstStage());
}

Game* Scenario::NewGame(const string& id, const string& first_stage_script) {
  const Path path = GameEnv::GetInstance()->GetScenarioPath() / id / first_stage_script;
  return new Game(rc_, assets_, path);
}

Game* Scenario::LoadGame(const string& scenario_id, const string& save_file_path) {
  UNREACHABLE("NYI");
//  return new Game(rc_, assets_, GameEnv::GetInstance()->GetScenarioPath() + "/" + base_path + "/" + first_stage_script);
  return nullptr;
}

Scenario::~Scenario() {
  delete rc_.unit_class_manager;
  delete rc_.terrain_manager;
  delete rc_.magic_manager;
  delete rc_.equipment_manager;
  delete rc_.hero_tpl_manager;

  delete assets_;
  delete game_;
}

} // namespace core
} // namespace mengde
