#ifndef WINDOW_VIEW_H_
#define WINDOW_VIEW_H_

#include <functional>
#include <vector>
#include "util/common.h"
#include "gui/foundation/timer.h"
#include "gui/foundation/event_fetcher.h"

namespace mengde {
namespace core {
  class Game;
  class Scenario;
}
}

class Drawer;
class EventFetcher;
class View;
class Window;

class FrameConfig {
 public:
  FrameConfig(uint16_t /* max_frames_sec */ = 60, float /* speed */ = 1);
  uint16_t GetMaxFps() const { return max_frames_sec_; }
  uint16_t GetDelay() const { return (1000.0f / speed_) / static_cast<float>(max_frames_sec_); }
  uint32_t MsecToFrame(uint32_t) const;
  uint32_t SecToFrame(uint32_t) const;

 private:
  uint16_t max_frames_sec_;
  float    speed_;
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

class App {
 public:
  App(int, int, uint32_t /* max_frame_sec */ = 60);
  ~App();

 public:
  void Run();

  // Getters and Setters
  Drawer* GetDrawer();
  Vec2D GetWindowSize() { return window_size_; }
  uint16_t GetMaxFps() { return frame_config_.GetMaxFps(); }
  uint32_t MsecToFrame(uint32_t ms) { return frame_config_.MsecToFrame(ms); }
  void SetMagicListViewVisible(bool);
  void SetQuit(bool b) { quit_ = b; }
  void EndGame();

  void StartNewGame();

 public:
  static const int kBlockSize = 48;

 private:
  void HandleEvents();
  void Update();
  void Render();

 private:
  EventFetcher event_fetcher_;
  Vec2D   window_size_;
  Window* window_;
  Drawer* drawer_;
  View*   main_view_;
  View*   root_view_;
  View*   target_view_;
  mengde::core::Scenario* scenario_;

  // fps
  const FrameConfig frame_config_;
  FpsTimer fps_timer_;

  bool quit_;
};

#endif
