#ifndef MENGDE_CORE_DESERIALIZER_H_
#define MENGDE_CORE_DESERIALIZER_H_

#include "assets.h"
#include "hero.h"
#include "scenario.h"
#include "scenario_generated.h"
#include "util/common.h"
#include "util/path.h"

namespace mengde {
namespace core {

class Deserializer {
 public:
  Deserializer(const Path& path);
  unique_ptr<Scenario> Deserialize();

 private:
  ResourceManagers Build(const save::ResourceManagers& rm);
  HeroClassManager* Build(const flatbuffers::Vector<flatbuffers::Offset<save::HeroClass>>& hcm);

  template <typename CoreStruct, typename FbsStruct>
  CoreStruct BuildStruct(const FbsStruct& fs) {
    static_assert(sizeof(FbsStruct) == sizeof(CoreStruct), "struct size mismatches");
    return *reinterpret_cast<const CoreStruct*>(&fs);
  }

 private:
  Path path_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_DESERIALIZER_H_
