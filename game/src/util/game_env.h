#ifndef GAME_ENV_H_
#define GAME_ENV_H_

#include "path.h"

class GameEnv {
 public:
  static GameEnv* GetInstance();
  Path GetGamePath() { return game_path_; }
  Path GetResourcePath() { return resource_path_; }
  Path GetScenarioPath() { return scenario_path_; }

 private:
  GameEnv();

 private:
  Path game_path_;
  Path resource_path_;
  Path scenario_path_;
};

#endif // GAME_ENV_H_
