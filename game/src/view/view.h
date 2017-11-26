#ifndef VIEW_H_
#define VIEW_H_

#include "util/common.h"
#include "rect.h"
#include "i_controller.h"
#include "i_view.h"

class View : public IView, public IController {
 public:
  View();
  View(const Rect*);
  virtual ~View() {}
  const Rect* GetFrame() { return &frame_; }
  Rect GetActualFrame() const;
  Vec2D GetFrameSize() const;
  Vec2D GetActualFrameSize() const;
  Vec2D GetFrameCoords() const;
  Vec2D GetActualFrameCoords() const;
  void SetCoords(Vec2D v) { frame_.SetPos(v); }
  bool IsVisible() const { return visible_; }
  int  GetPadding() { return padding_; }
  void SetFrame(const Rect* r) { frame_ = *r; }
  void SetBgColor(Color c) { bg_color_ = c; }
  void SetPadding(int p) { padding_ = p; }
  void SetVisible(bool b) { visible_ = b; }
  bool DelegateMouseButtonEvent(const MouseButtonEvent);
  bool DelegateMouseMotionEvent(const MouseMotionEvent);

 public:
  virtual void Render(Drawer*) override final;
  virtual void Update() override {}
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override { return false; }
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override { return false; }

 private:
  virtual void RenderView(Drawer*) = 0;

 private:
  Rect frame_;
  Color bg_color_;
  int padding_;
  uint32_t visible_:1;
  uint32_t reserved:31;
};

#endif
