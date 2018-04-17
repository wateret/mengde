#ifndef MENGDE_GUI_FOUNDATION_SURFACE_H_
#define MENGDE_GUI_FOUNDATION_SURFACE_H_

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

}  // namespace foundation
}  // namespace gui
}  // namespace mengde

#endif // MENGDE_GUI_FOUNDATION_SURFACE_H_
