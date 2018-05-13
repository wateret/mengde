#include "game_env.h"
#include "SDL.h"

GameEnv::GameEnv() {
  char* basepath = SDL_GetBasePath();
  game_path_ = Path(string(basepath));
  SDL_free(basepath);

  resource_path_ = game_path_ / "res";
  scenario_path_ = game_path_ / "sce";
}

const GameEnv* GameEnv::GetInstance() {
  static GameEnv instance;
  return &instance;
}
