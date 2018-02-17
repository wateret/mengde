#ifndef VIEW_APP_MISC_H_
#define VIEW_APP_MISC_H_

#include <string>

#include "gui/uifw/sprite_type.h"

#include "common.h"

namespace mengde {
namespace gui {
namespace app {

std::string GetModelPath(const std::string& model_id, SpriteType type);

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // VIEW_APP_MISC_H_
