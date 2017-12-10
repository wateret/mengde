#include "app.h"
#include "common.h"
#include "misc.h"
#include "texture.h"
#include "rect.h"
#include "color.h"
#include "unit.h"
#include "timer.h"
#include "drawer.h"
#include "main_view.h"
#include "root_view.h"
#include "game.h"
#include "config_loader.h"

App::App(int width, int height)
    : window_size_(width, height),
      drawer_(nullptr),
      main_view_(nullptr),
      root_view_(nullptr),
      target_view_(nullptr),
      game_(nullptr),
      frames_sec_(0),
      frames_total_(0),
      fps_(0.0),
      quit_(false) {
  Misc::Init();

  // Set texture filtering to linear
  if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") == 0) {
    throw "Error while SDL_SetHint";
  }

  Rect main_rect({0, 0}, window_size_);
  main_view_ = new MainView(&main_rect, this);

  ConfigLoader loader("config.lua");
  game_ = new Game(&loader);
  window_ = new Window("Game", width, height);
  drawer_ = new Drawer(window_);
  root_view_ = new RootView(window_size_, game_, this);
//  target_view_ = main_view_;
  target_view_ = root_view_;
}

App::~App() {
  ASSERT(main_view_ != nullptr);
  ASSERT(drawer_ != nullptr);
  ASSERT(window_ != nullptr);
  if (root_view_ != nullptr) {
    delete root_view_;
  }
  delete main_view_;
  delete drawer_;
  delete window_;
  if (game_ != nullptr) {
    delete game_;
  }
  Misc::Quit();
}

Drawer* App::GetDrawer() {
  return drawer_;
}

void App::Run() {
  const int kDelayTime = 1000.0f / kMaxFPS;

//  Rect main_view = {0, 0, kMainViewWidth, kMainViewHeight};
//  renderer_->SetViewport(&main_view);

  Timer frame_cap_timer;
  Timer fps_timer;

  fps_timer.Start();

  // Main event loop
  while (!quit_)
  {
    frame_cap_timer.Start();

    quit_ = HandleEvents();

    // TODO Refactoring - Move this to Update() {
    int time_elapsed = fps_timer.Split();
    if (time_elapsed >= 1000) {
      fps_ = frames_sec_ / (time_elapsed / 1000.f);
      frames_sec_ = 0;
      fps_timer.Start();
    }
    frames_sec_++;
    // }

    Update();
    Render();

    int frame_time = frame_cap_timer.Stop();
    if (frame_time < kDelayTime) {
      Misc::Delay(kDelayTime - frame_time);
    }
  }

}

bool App::HandleEvents() {
  SDL_Event e; // Event handler
  while (SDL_PollEvent(&e) != 0)
  {
    switch (e.type) {
      case SDL_QUIT: {
        quit_ = true;
        break;
      }
      case SDL_MOUSEMOTION: {
        Vec2D coords = {e.motion.x, e.motion.y};
        Vec2D coords_rel = {e.motion.xrel, e.motion.yrel};
        target_view_->OnMouseMotionEvent(MouseMotionEvent(MouseMotionEvent::Type::kOver, coords, coords_rel));
        break;
      }
      case SDL_MOUSEBUTTONUP:
      case SDL_MOUSEBUTTONDOWN: {
        MouseButtonEvent::State state = (e.type == SDL_MOUSEBUTTONUP) ?
                                        MouseButtonEvent::kStateUp :
                                        MouseButtonEvent::kStateDown;
        MouseButtonEvent::Button button = MouseButtonEvent::kButtonNone;
        switch (e.button.button) {
          case SDL_BUTTON_LEFT:
            button = MouseButtonEvent::kButtonLeft;
            break;
          case SDL_BUTTON_MIDDLE:
            button = MouseButtonEvent::kButtonMiddle;
            break;
          case SDL_BUTTON_RIGHT:
            button = MouseButtonEvent::kButtonRight;
            break;
          default:
            UNREACHABLE("Unknown mouse button");
            break;
        }
        target_view_->OnMouseButtonEvent(MouseButtonEvent(button, state, Vec2D(e.button.x, e.motion.y)));
        break;
      }
      case SDL_MOUSEWHEEL: {
        int32_t x, y;
        SDL_GetMouseState(&x, &y);
        MouseWheelEvent::Horizontal hor = MouseWheelEvent::Horizontal::kNone;
        if (e.wheel.x < 0) hor = MouseWheelEvent::Horizontal::kLeft;
        if (e.wheel.x > 0) hor = MouseWheelEvent::Horizontal::kRight;
        MouseWheelEvent::Vertical ver = MouseWheelEvent::Vertical::kNone;
        if (e.wheel.y < 0) ver = MouseWheelEvent::Vertical::kDown;   // TODO check the direction
        if (e.wheel.y > 0) ver = MouseWheelEvent::Vertical::kUp;
        target_view_->OnMouseWheelEvent(MouseWheelEvent(hor, ver, Vec2D(x, y)));
      }
      default:
        break;
    }
  }
  return quit_;
}

void App::Update() {
  target_view_->Update();

  frames_total_++;
}

void App::Render() {
  drawer_->Start();

  target_view_->Render(drawer_);

  // Print message (fps, ...)
#if 1
  static char msg_buf[256];
  sprintf(msg_buf, "fps: %.1f", fps_);
  drawer_->DrawText(msg_buf, 12, COLOR_WHITE, {0, 0});
#endif

  drawer_->End();
}

void App::StartNewGame() {
  target_view_ = root_view_;
}

void App::EndGame() {
  // FIXME We can't delete these objects now since this function is called from
  //       these object methods.
//  delete root_view_;
//  delete game_;

  root_view_ = nullptr;
  game_ = nullptr;

  target_view_ = main_view_;
}

