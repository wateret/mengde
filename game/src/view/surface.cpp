#include "surface.h"


Surface::Surface(SDL_Surface* surface) : handle_(surface) {
}


SDL_Surface* Surface::getRawSurface() {
  return handle_;
}
