#include "resource_path.h"

namespace mengde {
namespace gui {
namespace app {

namespace rcpath {

static const char kBitmapExt[] = ".bmp";

Path MapPath(const std::string& id) {
  static const Path map_dir = Path{Path("gui") / Path("map")};
  return map_dir / Path(id + kBitmapExt);
}

Path UnitModelPath(const std::string& id) {
  static const Path model_dir = Path{Path("gui") / Path("model")};
  return model_dir / id;
}

Path UnitModelPath(const std::string& id, SpriteType type) {
  return UnitModelPath(id) / (Path(std::string(kSpriteBitmapName[type]) + kBitmapExt));
}

Path EquipmentModelPath(const std::string& id) {
  static const Path equipment_dir = Path{Path("gui") / Path("equipment")};
  return equipment_dir / Path(id + kBitmapExt);
}

Path PortraitPath(const std::string& id) {
  static const Path portrait_dir = Path{Path("gui") / Path("portrait")};
  return portrait_dir / Path(id + kBitmapExt);
}

Path MagicPath(const std::string& id) {
  static const Path magic_dir = Path{Path("gui") / Path("magic")};
  return magic_dir / Path(id + kBitmapExt);
}

}  // namespace rcpath

}  // namespace app
}  // namespace gui
}  // namespace mengde
