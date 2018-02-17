#include "window.h"

namespace mengde {
namespace gui {
namespace foundation {

Window::Window(const std::string& title, int width, int height) {
  handle_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
                             SDL_WINDOW_SHOWN);
  if (handle_ == NULL) {
    throw "SDL_CreateWindow Error";
  }
}

Window::~Window() { SDL_DestroyWindow(handle_); }

SDL_Window* Window::AsRawWindow() { return handle_; }

void Window::Update() {
  // Update the surface
  SDL_UpdateWindowSurface(handle_);
}

void Window::InfoMessageBox(const char* title, const char* message) {
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, title, message, handle_);
}

Vec2D Window::GetSize() {
  Vec2D size;
  SDL_GetWindowSize(handle_, &size.x, &size.y);
  return size;
}

}  // namespace foundation
}  // namespace gui
}  // namespace mengde
