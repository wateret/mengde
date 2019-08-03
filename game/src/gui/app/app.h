#ifndef MENGDE_GUI_APP_APP_H_
#define MENGDE_GUI_APP_APP_H_

#include <functional>
#include <vector>
#include "gui/foundation/event_fetcher.h"
#include "gui/foundation/timer.h"
#include "util/common.h"
#include "util/path.h"

#include "common.h"

namespace mengde {
namespace core {

class Stage;
class Scenario;

}  // namespace core
}  // namespace mengde

namespace mengde {
namespace gui {
namespace app {

class FrameConfig {
 public:
  FrameConfig(uint16_t /* max_frames_sec */ = 60, float /* speed */ = 1);
  uint16_t GetMaxFps() const { return max_frames_sec_; }
  uint16_t GetDelay() const { return (1000.0f / speed_) / static_cast<float>(max_frames_sec_); }
  uint32_t MsecToFrame(uint32_t) const;
  uint32_t SecToFrame(uint32_t) const;

 private:
  uint16_t max_frames_sec_;
  float speed_;
};

class FpsTimer {
 public:
  FpsTimer() : timer_(), frames_cur_sec_(0), fps_(0.f) {}
  void Start();
  void Update();
  float GetLastFps() { return fps_; }

 private:
  Timer timer_;
  uint32_t frames_cur_sec_;
  float fps_;
};

class MainView;
class RootView;

class App {
 public:
  typedef function<void()> NextFrameCallback;

  App(int, int, uint32_t /* max_frame_sec */ = 60);
  ~App();

 public:
  void Run();

  // Getters and Setters
  Path GetCurrentScenarioPath() const;
  Drawer* GetDrawer();
  Vec2D GetWindowSize() { return window_size_; }
  uint16_t GetMaxFps() { return frame_config_.GetMaxFps(); }
  uint32_t MsecToFrame(uint32_t ms) { return frame_config_.MsecToFrame(ms); }
  void SetMagicListViewVisible(bool);
  void SetQuit(bool b) { quit_ = b; }
  void EndStage();

  void StartNewScenario(const string& scenario_id);
  void LoadScenario(const Path& savefile_path);

  void NextFrame(NextFrameCallback);

 private:
  void HandleEvents();
  void Update();
  void Render();

  void RunCallbacks();

 private:
  EventFetcher event_fetcher_;
  Vec2D window_size_;
  Window* window_;
  Drawer* drawer_;
  MainView* main_view_;
  RootView* root_view_;
  View* target_view_;
  core::Scenario* scenario_;

  queue<NextFrameCallback> frame_callbacks_;

  // fps
  const FrameConfig frame_config_;
  FpsTimer fps_timer_;

  bool quit_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_APP_H_
