#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <SDL.h>
#include "surface.h"
#include "common.h"

class Window {
 public:
  Window(const std::string&, int, int);
  ~Window();
  void Update();
  void InfoMessageBox(const char* title, const char* message);
  SDL_Window* AsRawWindow();
  Vec2D GetSize();

 private:
  SDL_Window* handle_;
};

#endif
