#include "surface.h"


namespace mengde {
namespace gui {
namespace foundation {

Surface::Surface(SDL_Surface* surface) : handle_(surface) {
}


SDL_Surface* Surface::getRawSurface() {
  return handle_;
}

} // namespace foundation
} // namespace gui
} // namespace mengde
