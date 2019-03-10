#ifndef MENGDE_CORE_SAVE_FILE_H_
#define MENGDE_CORE_SAVE_FILE_H_

#include <memory>

#include "scenario.h"
#include "scenario_generated.h"
#include "util/path.h"

namespace mengde {
namespace core {

class SaveFile {
 public:
  SaveFile(const Path& path);
  void Serialize(const int* scenario);
  int* Deserialize();

 private:
  Path path_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_SAVE_FILE_H_
