#include "app.h"

#include "core/assets.h"
#include "core/deserializer.h"
#include "core/exceptions.h"
#include "core/scenario.h"
#include "core/stage.h"
#include "core/unit.h"
#include "gui/foundation/color.h"
#include "gui/foundation/event_fetcher.h"
#include "gui/foundation/misc.h"
#include "gui/foundation/rect.h"
#include "gui/foundation/texture.h"
#include "gui/foundation/window.h"
#include "gui/uifw/drawer.h"
#include "main_view.h"
#include "root_view.h"
#include "util/common.h"
#include "util/game_env.h"

namespace mengde {
namespace gui {
namespace app {

FrameConfig::FrameConfig(uint16_t max_frames_sec, float speed) : max_frames_sec_(max_frames_sec), speed_(speed) {
  ASSERT(0.125f <= speed_ && speed_ <= 4.0f);
  LOG_DEBUG("FPS CAP : %d", max_frames_sec_);
  LOG_DEBUG("SPEED   : %fx", speed_);
}

uint32_t FrameConfig::MsecToFrame(uint32_t ms) const { return SecToFrame(ms) / 1000; }

uint32_t FrameConfig::SecToFrame(uint32_t s) const { return s * static_cast<uint32_t>(max_frames_sec_); }

void FpsTimer::Start() { timer_.Start(); }

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
    : event_fetcher_(),
      window_size_(width, height),
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
  // TODO Do not do this here (No direct SDL API allowed)
  if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") == 0) {
    throw "Error while SDL_SetHint";
  }

  Rect main_rect({0, 0}, window_size_);
  main_view_ = new MainView(main_rect, this);
  window_ = new Window("Stage", window_size_.x, window_size_.y);
  drawer_ = new Drawer(window_, GameEnv::GetInstance()->GetScenarioPath().ToString(),
                       GameEnv::GetInstance()->GetResourcePath().ToString());

  target_view_ = main_view_;
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

Path App::GetCurrentScenarioPath() const { return GameEnv::GetInstance()->GetScenarioPath() / scenario_->id(); }

Drawer* App::GetDrawer() { return drawer_; }

void App::Run() {
  static const int kDelayTime = frame_config_.GetDelay();

  Timer frame_cap_timer;

  fps_timer_.Start();

  // Main event loop
  while (!quit_) {
    frame_cap_timer.Start();

    HandleEvents();

    fps_timer_.Update();
    Update();
    Render();

    int frame_time = frame_cap_timer.Stop();
    if (frame_time < kDelayTime) {
      Misc::Delay(kDelayTime - frame_time);
    }
  }
}

void App::HandleEvents() {
  while (event_fetcher_.Poll()) {
    const Event& e = event_fetcher_.event();
    switch (event_fetcher_.event_type()) {
      case EventType::kQuit:
        quit_ = true;
        break;
      case EventType::kMouseMotion:
        target_view_->OnMouseMotionEvent(e.mouse_motion);
        break;
      case EventType::kMouseButton:
        target_view_->OnMouseButtonEvent(e.mouse_button);
        break;
      case EventType::kMouseWheel:
        target_view_->OnMouseWheelEvent(e.mouse_wheel);
        break;
      case EventType::kKey:
        target_view_->OnKeyEvent(e.key);
        break;
      default:
        break;
    }
  }
}

void App::Update() {
  RunCallbacks();
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

void App::StartNewScenario(const string& scenario_id) {
  try {
    scenario_ = new core::Scenario(scenario_id);
  } catch (const core::ConfigLoadException& e) {
    // TODO Show error message appropriately
    LOG_FATAL("Scenario config load failure - %s", e.what());
    UNREACHABLE("Scenario config load failure.");
  }

  root_view_ = new RootView(Rect({0, 0}, window_size_), scenario_, this);
  drawer_->SetBitmapBasePath(GetCurrentScenarioPath().ToString());

  target_view_ = root_view_;
}

void App::LoadScenario(const Path& savefile_path) {
  try {
    core::Deserializer deserializer{savefile_path};
    auto scenario = deserializer.Deserialize();
    scenario_ = scenario.release();
  } catch (const core::ConfigLoadException& e) {
    // TODO Show error message appropriately
    LOG_FATAL("Savefile load failure - %s", e.what());
    UNREACHABLE("Savefile load failure.");  // TODO Change this to throw
  }

  root_view_ = new RootView(Rect({0, 0}, window_size_), scenario_, this);
  drawer_->SetBitmapBasePath(GetCurrentScenarioPath().ToString());

  target_view_ = root_view_;
}

void App::EndStage() {
  delete root_view_;
  bool has_next = scenario_->NextStage();

  if (has_next) {
    root_view_ = new RootView(Rect({0, 0}, window_size_), scenario_, this);
    target_view_ = root_view_;
  } else {
    root_view_ = nullptr;
    target_view_ = main_view_;

    delete scenario_;
    scenario_ = nullptr;
  }

  /* For the case of end of the scenario
  // FIXME We can't delete these objects now since this function is called from
  //       these object methods.
  //  delete root_view_;

  root_view_   = nullptr;
  target_view_ = main_view_;
  */
}

void App::NextFrame(NextFrameCallback cb) { frame_callbacks_.push(cb); }

void App::RunCallbacks() {
  while (!frame_callbacks_.empty()) {
    frame_callbacks_.front()();
    frame_callbacks_.pop();
  }
}

}  // namespace app
}  // namespace gui
}  // namespace mengde
