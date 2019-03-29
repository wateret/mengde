#ifndef MENGDE_CORE_SAVE_FILE_H_
#define MENGDE_CORE_SAVE_FILE_H_

#include <memory>

#include "scenario.h"
#include "scenario_generated.h"
#include "util/path.h"

namespace mengde {
namespace core {

class Scenario;

class SaveFile {
 public:
  SaveFile(const Path& path);

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
  const save::BaseIncr* Build(const BaseAndIncr& bni);
  const save::Attribute* Build(const Attribute& attr);
  const save::TurnBased* Build(const TurnBased& turn_based);
  const save::StatMod* Build(const StatMod& stat_mod);

 public:
  void Deserialize();

 private:
  Path path_;
  flatbuffers::FlatBufferBuilder builder_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_SAVE_FILE_H_
