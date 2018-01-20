#ifndef SURFACE_H
#define SURFACE_H

#include <SDL.h>

namespace mengde {
namespace gui {
namespace foundation {

class Surface {
 public:
  Surface(SDL_Surface*);

  SDL_Surface* getRawSurface();

 private:
  SDL_Surface* handle_;
};

} // namespace foundation
} // namespace gui
} // namespace mengde

#endif
