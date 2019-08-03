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
  assets_ = std::make_unique<Assets>();
  current_stage_ = NewStage(stage_ids_[stage_no_]);
}

Scenario::Scenario(const string& scenario_id, const vector<string>& stage_ids, uint32_t stage_no,
                   const ResourceManagers& rc, unique_ptr<Assets>&& assets)
    : scenario_id_{scenario_id}, stage_ids_{stage_ids}, stage_no_{stage_no}, rc_{rc}, assets_{std::move(assets)}, current_stage_{NewStage(stage_ids_[stage_no_])} {}

unique_ptr<Stage> Scenario::NewStage(const string& stage_id) {
  const Path path = GameEnv::GetInstance()->GetScenarioPath() / scenario_id_ / "script" / (stage_id + ".lua");
  return std::make_unique<Stage>(rc_, assets_.get(), path);
}

bool Scenario::NextStage() {
  auto&& stage_assets = current_stage_->ReturnAssets();
  ASSERT(assets_.get() != stage_assets.get());

  // Update Assets
  assets_ = std::move(stage_assets);

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
  delete rc_.hero_class_manager;
  delete rc_.terrain_manager;
  delete rc_.magic_manager;
  delete rc_.equipment_manager;
  delete rc_.hero_tpl_manager;
}

}  // namespace core
}  // namespace mengde
