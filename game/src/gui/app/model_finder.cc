#include "model_finder.h"

#include "resource_path.h"

namespace mengde {
namespace gui {
namespace app {

string FindModelId(const Path& base_path, const string& hero_class, const string& hero_id, const core::Force& force) {
  auto force_str = (force == core::Force::kOwn) ? "own" : ((force == core::Force::kAlly) ? "ally" : "enemy");

  const vector<string> candidates{hero_class + "-" + hero_id,    // Try something with hero_id first
                                  hero_id,                       // Try something with hero_id first
                                  hero_class + "-" + force_str,  // Try general ones
                                  hero_class};                   // Try general ones

  string model_id{"Default"};
  for (const auto& cand : candidates) {
    if ((base_path / rcpath::UnitModelPath(cand)).Exists()) {
      model_id = cand;
      break;
    }
  }
  return model_id;
}

}  // namespace app
}  // namespace gui
}  // namespace mengde
