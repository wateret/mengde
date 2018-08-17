#include "scenario.h"

#include "assets.h"
#include "config_loader.h"
#include "stage.h"

namespace mengde {
namespace core {

Scenario::Scenario(const string& scenario_id)
    : scenario_id_(scenario_id), stage_ids_(), stage_no_(0), rc_(), assets_(nullptr) {
  const Path base_path(scenario_id_);

  ConfigLoader loader(base_path / "script" / "config.lua");
  rc_ = loader.GetResources();
  stage_ids_ = loader.GetStages();

  // For the case of NEW GAME
  assets_ = new Assets();
  current_stage_ = NewStage(stage_ids_[stage_no_]);
}

unique_ptr<Stage> Scenario::NewStage(const string& stage_id) {
  const Path path = GameEnv::GetInstance()->GetScenarioPath() / scenario_id_ / "script" / (stage_id + ".lua");
  return std::make_unique<Stage>(rc_, assets_, path);
}

bool Scenario::NextStage() {
  // FIXME Currently the below condition is always false.
  //       Need to change Lua API `on_deploy(game)` to `on_deploy(assets)
  // ASSERT(assets_ != stage_->assets());
  ASSERT(assets_ == current_stage_->assets());  // FIXME Remove this temporary condition

  // Update Assets
  // delete assets_;
  assets_ = current_stage_->assets();

  // Advance to the next stage
  if (stage_no_ < stage_ids_.size() - 1) {
    current_stage_ = NewStage(stage_ids_[++stage_no_]);
    return true;
  } else {
    current_stage_ = nullptr;
    return false;
  }
}

Scenario::~Scenario() {
  delete rc_.unit_class_manager;
  delete rc_.terrain_manager;
  delete rc_.magic_manager;
  delete rc_.equipment_manager;
  delete rc_.hero_tpl_manager;

  delete assets_;
}

}  // namespace core
}  // namespace mengde
