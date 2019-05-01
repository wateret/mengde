#ifndef MENGDE_CORE_DESERIALIZER_H_
#define MENGDE_CORE_DESERIALIZER_H_

#include "util/path.h"
#include "util/common.h"
#include "scenario.h"

namespace mengde {
namespace core {

class Deserializer {
 public:
  Deserializer(const Path& path);
  unique_ptr<Scenario> Deserialize();

 private:
  Path path_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_DESERIALIZER_H_
