#include "misc.h"
#include <SDL.h>
#include <SDL_ttf.h>

void Misc::Init() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw "SDL_Init Error";
  }
}

void Misc::Quit() {
  SDL_Quit();
}

void Misc::Delay(int ms) {
  SDL_Delay(ms);
}

std::string Misc::GetErrorMessage() {
  return std::string(SDL_GetError());
}

void Misc::SetShowCursor(bool v) {
  SDL_ShowCursor(v ? SDL_ENABLE : SDL_DISABLE);
}
