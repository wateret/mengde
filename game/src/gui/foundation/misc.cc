#include "misc.h"

#include <SDL.h>
#include <SDL_ttf.h>

namespace mengde {
namespace gui {
namespace foundation {

void Misc::Init() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw "SDL_Init Error";
  }

  // metal renderer has priority
  SDL_SetHintWithPriority(SDL_HINT_RENDER_DRIVER, "metal", SDL_HINT_OVERRIDE);
}

void Misc::Quit() { SDL_Quit(); }

void Misc::Delay(int ms) { SDL_Delay(ms); }

std::string Misc::GetErrorMessage() { return std::string(SDL_GetError()); }

void Misc::SetShowCursor(bool v) { SDL_ShowCursor(v ? SDL_ENABLE : SDL_DISABLE); }

}  // namespace foundation
}  // namespace gui
}  // namespace mengde
