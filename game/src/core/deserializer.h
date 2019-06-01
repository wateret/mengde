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
  TerrainManager* Build(const flatbuffers::Vector<flatbuffers::Offset<save::Terrain>>& tm);
  HeroClassManager* Build(const flatbuffers::Vector<flatbuffers::Offset<save::HeroClass>>& hcm);
  MagicManager* Build(const flatbuffers::Vector<flatbuffers::Offset<save::Magic>>& mm);
  EquipmentManager* Build(const flatbuffers::Vector<flatbuffers::Offset<save::Equipment>>& em);
  HeroTemplateManager* Build(const flatbuffers::Vector<flatbuffers::Offset<save::HeroTemplate>>& htm,
                             const HeroClassManager& hcm);
  MagicEffect* Build(const save::MagicEffect& me);
  VolatileAttribute Build(const save::VolatileAttributes& va);
  EventEffectBase* Build(const save::EventEffect& ee);
  AttributeModifier Build(const save::AttributeModifier& mod);

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
