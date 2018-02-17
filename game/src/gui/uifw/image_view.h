#ifndef IMAGE_VIEW_H_
#define IMAGE_VIEW_H_

#include <string>
#include "util/common.h"
#include "view.h"

#include "common.h"

namespace mengde {
namespace gui {
namespace uifw {

class Drawer;

class ImageView : public View {
 public:
  ImageView(const Rect*, const string& = "");
  ImageView(const Rect&, const string& = "");
  ~ImageView();
  void SetPath(const string&);
  void SetSourceRect(const Rect&);

  virtual void Render(Drawer*) override;
  virtual bool OnMouseButtonEvent(const foundation::MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const foundation::MouseMotionEvent) override;

 private:
  std::string path_;
  Texture*    texture_;
  Rect        src_rect_;
};

}  // namespace uifw
}  // namespace gui
}  // namespace mengde

#endif  // IMAGE_VIEW_H_
