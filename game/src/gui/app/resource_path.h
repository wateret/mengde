#ifndef MENGDE_GUI_APP_RESOURCE_PATH_H_
#define MENGDE_GUI_APP_RESOURCE_PATH_H_

#include <string>

#include "gui/uifw/sprite_type.h"
#include "util/path.h"

#include "common.h"

namespace mengde {
namespace gui {
namespace app {

namespace rcpath {

Path MapPath(const std::string& id);
Path UnitModelPath(const std::string& id, SpriteType type);
Path EquipmentModelPath(const std::string& id);
Path PortraitPath(const std::string& id);
Path MagicPath(const std::string& id);

}  // namespace rcpath

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_RESOURCE_PATH_H_
