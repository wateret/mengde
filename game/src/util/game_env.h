#ifndef GAME_ENV_H_
#define GAME_ENV_H_

#include <string>

class GameEnv {
 public:
  static GameEnv* GetInstance();
  std::string GetGamePath() { return game_path_; }
  std::string GetResourcePath() { return resource_path_; }
  std::string GetScenarioPath() { return scenario_path_; }

 private:
  GameEnv();

 private:
  std::string game_path_;
  std::string resource_path_;
  std::string scenario_path_;
};

#endif // GAME_ENV_H_
