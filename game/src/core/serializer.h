#ifndef MENGDE_CORE_SERIALIZER_H_
#define MENGDE_CORE_SERIALIZER_H_

#include <memory>

#include "scenario.h"
#include "scenario_generated.h"
#include "util/path.h"

namespace mengde {
namespace core {

class Scenario;

class Serializer {
 public:
  Serializer(const Path& path);

 public:
  void Serialize(const Scenario& scenario);

 private:
  flatbuffers::Offset<save::Scenario> Build(const Scenario& scenario);
  flatbuffers::Offset<save::ResourceManagers> Build(const ResourceManagers& rm);
  flatbuffers::Offset<save::HeroClassManager> Build(const HeroClassManager& hcm);
  flatbuffers::Offset<save::HeroTemplateManager> Build(const HeroTemplateManager& htm);
  flatbuffers::Offset<save::EquipmentManager> Build(const EquipmentManager& em);
  flatbuffers::Offset<save::HeroClass> Build(const HeroClass& hero_class);
  flatbuffers::Offset<save::HeroTemplate> Build(const HeroTemplate& hero_tpl);
  flatbuffers::Offset<save::TerrainManager> Build(const TerrainManager& tm);
  flatbuffers::Offset<save::TerrainRecord> Build(const string& id, const Terrain& terrain);
  flatbuffers::Offset<save::Terrain> Build(const Terrain& terrain);
  flatbuffers::Offset<save::Equipment> Build(const Equipment& equipment);
  flatbuffers::Offset<save::VolatileAttributes> Build(const VolatileAttribute& va);
  flatbuffers::Offset<save::AttributeModifierList> Build(const StatModifierList& aml);
  flatbuffers::Offset<save::AttributeModifier> Build(const StatModifier& am);
  flatbuffers::Offset<save::PromotionInfo> Build(const PromotionInfo& promotion_info);

  template <typename FbsStruct, typename CoreStruct>
  const FbsStruct* BuildStruct(const CoreStruct& cs) {
    static_assert(sizeof(FbsStruct) == sizeof(CoreStruct), "struct size mismatches");
    return reinterpret_cast<const FbsStruct*>(&cs);
  }

 private:
  Path path_;
  flatbuffers::FlatBufferBuilder builder_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_SERIALIZER_H_
