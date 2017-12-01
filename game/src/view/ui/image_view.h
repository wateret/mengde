#ifndef IMAGE_VIEW_H_
#define IMAGE_VIEW_H_

#include "view.h"
#include "common.h"
#include <string>

class Drawer;
class Texture;

class ImageView : public View {
 public:
  ImageView(const Rect*, const string& = "");
  ~ImageView();
  void SetPath(const string&);

  virtual void Render(Drawer*) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;

 private:
  std::string path_;
  Texture* texture_;
};

#endif // IMAGE_VIEW_H_
