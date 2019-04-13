#ifndef MENGDE_CORE_SERIALIZER_H_
#define MENGDE_CORE_SERIALIZER_H_

#include <memory>

#include "event_effect_list.h"
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
  flatbuffers::Offset<save::AttributeModifier> Build(const StatModifier& am);
  flatbuffers::Offset<save::PromotionInfo> Build(const PromotionInfo& promotion_info);
  flatbuffers::Offset<save::EventEffect> Build(const EventEffectBase& event_effect);
  flatbuffers::Offset<save::GeneralEventEffect> Build(const GeneralEventEffect& gee);
  flatbuffers::Offset<save::GEERestoreHp> Build(const GEERestoreHp& gee_restore_hp);
  flatbuffers::Offset<save::OnCmdEventEffect> Build(const OnCmdEventEffect& ocee);
  flatbuffers::Offset<save::OCEEPreemptiveAttack> Build(const OCEEPreemptiveAttack& ocee);
  flatbuffers::Offset<save::OCEEEnhanceBasicAttack> Build(const OCEEEnhanceBasicAttack& ocee);
  flatbuffers::Offset<save::Magic> Build(const Magic& magic);
  flatbuffers::Offset<save::MagicEffect> Build(const MagicEffect& magic_effect);
  flatbuffers::Offset<save::MagicEffectHp> Build(const MagicEffectHP& me_hp);
  flatbuffers::Offset<save::MagicEffectAttribute> Build(const MagicEffectStat& me_attr);
  flatbuffers::Offset<save::MagicEffectCondition> Build(const MagicEffectCondition& me_cond);

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
