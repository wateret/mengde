#include "game_env.h"
#include "SDL.h"

#include <stdio.h>
GameEnv::GameEnv() {
  char* basepath = SDL_GetBasePath();
  printf("%s\n", basepath);
  game_path_ = std::string(basepath);
  SDL_free(basepath);
  resource_path_ = game_path_ + "/res";
  scenario_path_ = game_path_ + "/sce/example";
}

GameEnv* GameEnv::GetInstance() {
  static GameEnv instance;
  return &instance;
}
