#include "scenario.h"

#include "assets.h"
#include "config_loader.h"
#include "game.h"

namespace mengde {
namespace core {

Scenario::Scenario(const string& scenario_id)
    : scenario_id_(scenario_id), stage_ids_(), stage_no_(0), rc_(), assets_(nullptr), game_(nullptr) {
  const Path base_path(scenario_id_);

  ConfigLoader loader(base_path / "config.lua");
  rc_ = loader.GetResources();
  stage_ids_ = loader.GetStages();

  // For the case of NEW GAME
  assets_ = new Assets();
  game_ = NewGame(stage_ids_[stage_no_]);
}

Game* Scenario::NewGame(const string& stage_id) {
  const Path path = GameEnv::GetInstance()->GetScenarioPath() / scenario_id_ / "stage" / (stage_id + ".lua");
  return new Game(rc_, assets_, path);
}

Game* Scenario::LoadGame(const string& save_file_path) {
  UNUSED(save_file_path);

  // TODO Support save files
  UNREACHABLE("NYI");
  return nullptr;
}

void Scenario::NextStage() {
  // FIXME Currently the below condition is always false.
  //       Need to change Lua API `on_deploy(game)` to `on_deploy(assets)
  // ASSERT(assets_ != game_->assets());

  // Update Assets
  // delete assets_;
  assets_ = game_->assets();

  // Advance to the next stage
  delete game_;
  game_ = NewGame(stage_ids_[++stage_no_]);
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

}  // namespace core
}  // namespace mengde
