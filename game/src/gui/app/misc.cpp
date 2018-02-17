#include "misc.h"

namespace mengde {
namespace gui {
namespace app {

std::string GetModelPath(const std::string& model_id, SpriteType type) {
  return "model/" + model_id + "/" + kSpriteBitmapName[type] + ".bmp";
}

}  // namespace app
}  // namespace gui
}  // namespace mengde
