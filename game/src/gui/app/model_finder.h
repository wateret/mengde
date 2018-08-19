#ifndef MENGDE_GUI_APP_MODEL_FINDER_H_
#define MENGDE_GUI_APP_MODEL_FINDER_H_

#include "core/force.h"
#include "util/common.h"
#include "util/path.h"

namespace mengde {
namespace gui {
namespace app {

string FindModelId(const Path& base_path, const string& hero_class, const string& hero_id, const core::Force& force);

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_MODEL_FINDER_H_
