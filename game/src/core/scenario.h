#ifndef SCENARIO_H_
#define SCENARIO_H_

#include "resource_manager.h"
#include "util/common.h"

namespace mengde {
namespace core {

class Assets;
class Game;

// Scenario data globally used during the entire scenario

class Scenario {
 public:
  Scenario(const string&);
  ~Scenario();
  const ResourceManagers& GetResourceManagers() { return rc_; }
  Assets*                 GetAssets() { return assets_; }
  Game*                   GetGame() { return game_; }

 public:
  void NextStage();

 private:
  Game* NewGame(const string& stage_id);
  Game* LoadGame(const string& save_file_path);

 private:
  string           scenario_id_;
  vector<string>   stage_ids_;
  uint32_t         stage_no_;
  ResourceManagers rc_;
  Assets*          assets_;
  Game*            game_;
};

}  // namespace core
}  // namespace mengde

#endif  // SCENARIO_H_
