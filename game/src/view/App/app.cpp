#include "app.h"
#include "core/assets.h"
#include "core/game.h"
#include "core/scenario.h"
#include "core/unit.h"
#include "util/common.h"
#include "view/Foundation/misc.h"
#include "view/Foundation/texture.h"
#include "view/Foundation/rect.h"
#include "view/Foundation/color.h"
#include "view/Foundation/drawer.h"
#include "main_view.h"
#include "root_view.h"

FrameConfig::FrameConfig(uint16_t max_frames_sec, float speed) : max_frames_sec_(max_frames_sec), speed_(speed) {
  ASSERT(0.125f <= speed_ && speed_ <= 4.0f);
  LOG_DEBUG("fps:%d speed:%f", max_frames_sec_, speed_);
}

uint32_t FrameConfig::MsecToFrame(uint32_t ms) const {
  return SecToFrame(ms) / 1000;
}

uint32_t FrameConfig::SecToFrame(uint32_t s) const {
  return s * static_cast<uint32_t>(max_frames_sec_);
}

void FpsTimer::Start() {
  timer_.Start();
}

void FpsTimer::Update() {
  int time_elapsed = timer_.Split();
  if (time_elapsed >= 1000) {
    fps_ = frames_cur_sec_ / (time_elapsed / 1000.f);
    frames_cur_sec_ = 0;
    timer_.Start();
  }
  frames_cur_sec_++;
}

App::App(int width, int height, uint32_t max_frames_sec)
    : window_size_(width, height),
      drawer_(nullptr),
      main_view_(nullptr),
      root_view_(nullptr),
      target_view_(nullptr),
      scenario_(nullptr),
      frame_config_(max_frames_sec, 2),
      fps_timer_(),
      quit_(false) {
  Misc::Init();

  // Set texture filtering to linear
  if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") == 0) {
    throw "Error while SDL_SetHint";
  }

  Rect main_rect({0, 0}, window_size_);
  main_view_ = new MainView(&main_rect, this);

  scenario_ = new Scenario("example");

  window_ = new Window("Game", width, height);
  drawer_ = new Drawer(window_);
  root_view_ = new RootView(window_size_, scenario_, this);
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
  if (scenario_ != nullptr) {
    delete scenario_;
  }
  Misc::Quit();
}

Drawer* App::GetDrawer() {
  return drawer_;
}

void App::Run() {
  static const int kDelayTime = frame_config_.GetDelay();

  Timer frame_cap_timer;

  fps_timer_.Start();

  // Main event loop
  while (!quit_)
  {
    frame_cap_timer.Start();

    quit_ = HandleEvents();

    fps_timer_.Update();
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
                                        MouseButtonEvent::State::kUp :
                                        MouseButtonEvent::State::kDown;
        MouseButtonEvent::Button button = MouseButtonEvent::Button::kNone;
        switch (e.button.button) {
          case SDL_BUTTON_LEFT:
            button = MouseButtonEvent::Button::kLeft;
            break;
          case SDL_BUTTON_MIDDLE:
            button = MouseButtonEvent::Button::kMiddle;
            break;
          case SDL_BUTTON_RIGHT:
            button = MouseButtonEvent::Button::kRight;
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
}

void App::Render() {
  drawer_->Start();

  target_view_->Render(drawer_);

  // Print message (fps, ...)
#if 1
  static char msg_buf[256];
  sprintf(msg_buf, "fps: %.1f", fps_timer_.GetLastFps());
  drawer_->DrawText(msg_buf, 12, COLOR("white"), {0, 0});
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
  target_view_ = main_view_;
}
