#ifndef GAME_ENV_H_
#define GAME_ENV_H_

#include "path.h"

class GameEnv {
 public:
  static const GameEnv* GetInstance();
  const Path& GetGamePath() const { return game_path_; }
  const Path& GetResourcePath() const { return resource_path_; }
  const Path& GetScenarioPath() const { return scenario_path_; }

 private:
  GameEnv();

 private:
  Path game_path_;
  Path resource_path_;
  Path scenario_path_;
};

#endif // GAME_ENV_H_
