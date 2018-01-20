#ifndef SURFACE_H
#define SURFACE_H

#include <SDL.h>

class Surface {
 public:
  Surface(SDL_Surface*);

  SDL_Surface* getRawSurface();

 private:
  SDL_Surface* handle_;
};

#endif
