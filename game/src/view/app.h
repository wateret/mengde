#ifndef WINDOW_VIEW_H_
#define WINDOW_VIEW_H_

#include <functional>
#include <vector>
#include "window.h"
#include "common.h"

class View;
class Game;
class Drawer;

class App {
 public:
  App(int, int);
  ~App();

 public:
  void Run();

  // Getters and Setters
  Drawer* GetDrawer();
  Vec2D GetWindowSize() { return window_size_; }
  void SetMagicListViewVisible(bool);
  void SetQuit(bool b) { quit_ = b; }
  void EndGame();

  void StartNewGame();

 public:
  static const int kBlockSize = 48;
  static const int kMaxFPS = 60;

 private:
  bool HandleEvents();
  void Update();
  void Render();

 private:
  Vec2D   window_size_;
  Window* window_;
  Drawer* drawer_;
  View*   main_view_;
  View*   root_view_;
  View*   target_view_;
  Game*   game_;

  // fps
  int   frames_sec_;
  int   frames_total_;
  float fps_;

  bool    quit_;
};

#endif
