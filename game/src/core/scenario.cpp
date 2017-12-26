#include "scenario.h"
#include "assets.h"
#include "config_loader.h"
#include "game.h"

Scenario::Scenario(const string& id) : rc_(), assets_(nullptr), game_(nullptr) {
  const string base_path = "/" + id;
  ConfigLoader loader(base_path + "/config.lua");
  rc_ = loader.GetResources();

  // For the case of NEW GAME
  assets_ = new Assets();
  game_ = NewGame(id, loader.GetFirstStage());
}

Game* Scenario::NewGame(const string& id, const string& first_stage_script) {
  const string base_path = GameEnv::GetInstance()->GetScenarioPath() + "/" + id;
  return new Game(rc_, assets_, base_path + "/" + first_stage_script);
}

Game* Scenario::LoadGame(const string&, const string&) {
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
