#include "resource_path.h"

namespace mengde {
namespace gui {
namespace app {

namespace rcpath {

static const char kBitmapExt[] = ".bmp";

Path UnitModelPath(const std::string& id, SpriteType type) {
  return Path("model") / id / Path(std::string(kSpriteBitmapName[type]) + kBitmapExt);
}

Path EquipmentModelPath(const std::string& id) { return Path("equipment") / Path(id + kBitmapExt); }

Path PortraitPath(const std::string& id) { return Path("portrait") / Path(id + kBitmapExt); }

Path MagicPath(const std::string& id) { return Path("magic") / Path(id + kBitmapExt); }

}  // namespace rcpath

}  // namespace app
}  // namespace gui
}  // namespace mengde
