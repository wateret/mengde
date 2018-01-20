#ifndef IMAGE_VIEW_H_
#define IMAGE_VIEW_H_

#include "view.h"
#include "util/common.h"
#include <string>

namespace mengde {
namespace gui {
namespace uifw {

class Drawer;
class Texture;

class ImageView : public View {
 public:
  ImageView(const Rect*, const string& = "");
  ImageView(const Rect&, const string& = "");
  ~ImageView();
  void SetPath(const string&);
  void SetSourceRect(const Rect&);

  virtual void Render(Drawer*) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent) override;
  virtual bool OnMouseMotionEvent(const MouseMotionEvent) override;

 private:
  std::string path_;
  Texture* texture_;
  Rect src_rect_;
};

} // namespace uifw
} // namespace gui
} // namespace mengde

#endif // IMAGE_VIEW_H_
