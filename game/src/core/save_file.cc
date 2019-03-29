#include "save_file.h"

#include <fstream>

#include "scenario.h"

namespace mengde {
namespace core {

SaveFile::SaveFile(const Path& path) : path_{path} {}

void SaveFile::Serialize(const Scenario& scenario) {
  builder_ = flatbuffers::FlatBufferBuilder{1024};

  auto sce = Build(scenario);

  builder_.Finish(sce);

  uint8_t* buffer = builder_.GetBufferPointer();
  uint32_t size = builder_.GetSize();

  std::ofstream file(path_.ToString(), std::ios::binary);
  file.write(reinterpret_cast<const char*>(buffer), size);
}

flatbuffers::Offset<save::Scenario> SaveFile::Build(const Scenario& scenario) {
  auto id_off = builder_.CreateString(scenario.id());
  auto string_offset_vec = builder_.CreateVectorOfStrings(scenario.stage_id_list());
  auto resource_managers_off = Build(scenario.GetResourceManagers());

  save::ScenarioBuilder sce_builder(builder_);

  sce_builder.add_id(id_off);
  sce_builder.add_stage_id_list(string_offset_vec);
  sce_builder.add_stage_no(scenario.stage_no());
  sce_builder.add_resource_managers(resource_managers_off);

  return sce_builder.Finish();
}

flatbuffers::Offset<save::ResourceManagers> SaveFile::Build(const ResourceManagers& rm) {
  auto tm = Build(*rm.terrain_manager);
  auto hcm = Build(*rm.hero_class_manager);
  auto em = Build(*rm.equipment_manager);
  auto htm = Build(*rm.hero_tpl_manager);
  return save::CreateResourceManagers(builder_, tm, hcm, em, htm);
}

flatbuffers::Offset<save::HeroClassManager> SaveFile::Build(const HeroClassManager& hcm) {
  std::vector<flatbuffers::Offset<save::HeroClass>> records;
  hcm.ForEach([&](const string& id, const HeroClass& hero_class) {
    ASSERT(id == hero_class.id());
    records.push_back(Build(hero_class));
  });
  return save::CreateHeroClassManager(builder_, builder_.CreateVector(records));
}

flatbuffers::Offset<save::HeroTemplateManager> SaveFile::Build(const HeroTemplateManager& htm) {
  std::vector<flatbuffers::Offset<save::HeroTemplate>> records;
  htm.ForEach([&](const string& id, const HeroTemplate& hero_tpl) {
    ASSERT(id == hero_tpl.id());
    records.push_back(Build(hero_tpl));
  });
  return save::CreateHeroTemplateManager(builder_, builder_.CreateVector(records));
}

flatbuffers::Offset<save::HeroClass> SaveFile::Build(const HeroClass& hero_class) {
  auto id_off = builder_.CreateString(hero_class.id());
  auto attr_inl = Build(hero_class.stat_grade());
  auto attack_range = static_cast<int>(hero_class.attack_range_enum());
  auto move = hero_class.move();
  auto hp_inl = Build(hero_class.bni_hp());
  auto mp_inl = Build(hero_class.bni_mp());
  auto pi_off = hero_class.promotion_info() ? Build(*hero_class.promotion_info()) : 0;
  return save::CreateHeroClass(builder_, id_off, attr_inl, attack_range, move, hp_inl, mp_inl, pi_off);
}

flatbuffers::Offset<save::HeroTemplate> SaveFile::Build(const HeroTemplate& hero_tpl) {
  auto id_off = builder_.CreateString(hero_tpl.id());
  auto hero_class_off = builder_.CreateString(hero_tpl.hero_class()->id());
  auto attr_inl = Build(hero_tpl.GetHeroStat());
  return save::CreateHeroTemplate(builder_, id_off, hero_class_off, attr_inl);
}

flatbuffers::Offset<save::TerrainManager> SaveFile::Build(const TerrainManager& tm) {
  std::vector<flatbuffers::Offset<save::TerrainRecord>> terrain_records;
  tm.ForEach([&](const string& id, const Terrain& terrain) { terrain_records.push_back(Build(id, terrain)); });
  auto terrain_records_off = builder_.CreateVector(terrain_records);
  return save::CreateTerrainManager(builder_, terrain_records_off);
}

flatbuffers::Offset<save::TerrainRecord> SaveFile::Build(const string& id, const Terrain& terrain) {
  auto id_off = builder_.CreateString(id);
  auto terrain_off = Build(terrain);
  return save::CreateTerrainRecord(builder_, id_off, terrain_off);
}

flatbuffers::Offset<save::Terrain> SaveFile::Build(const Terrain& terrain) {
  return save::CreateTerrainDirect(builder_, terrain.id().c_str(), &terrain.move_costs(), &terrain.class_effects());
}

flatbuffers::Offset<save::PromotionInfo> SaveFile::Build(const PromotionInfo& promotion_info) {
  return save::CreatePromotionInfoDirect(builder_, promotion_info.id.c_str(), promotion_info.level);
}

const save::BaseIncr* SaveFile::Build(const BaseAndIncr& bni) {
  static_assert(sizeof(save::BaseIncr) == sizeof(BaseAndIncr), "struct size mismatches");
  // TODO Maybe add some static_asserts to check the members' offset
  // NOTE Below is not possible since base_ is a private member
  // static_assert(offsetof(save::BaseIncr, base_) == offsetof(BaseAndIncr, base), "Struct layout mismatches");
  // static_assert(offsetof(save::BaseIncr, incr_) == offsetof(BaseAndIncr, incr), "Struct layout mismatches");

  return reinterpret_cast<const save::BaseIncr*>(&bni);
}

const save::Attribute* SaveFile::Build(const Attribute& attr) {
  static_assert(sizeof(save::Attribute) == sizeof(Attribute), "struct size mismatches");
  // TODO Maybe add some static_asserts to check the members' offset

  return reinterpret_cast<const save::Attribute*>(&attr);
}

const save::TurnBased* SaveFile::Build(const TurnBased& turn_based) {
  static_assert(sizeof(save::TurnBased) == sizeof(TurnBased), "struct size mismatches");

  return reinterpret_cast<const save::TurnBased*>(&turn_based);
}

const save::StatMod* SaveFile::Build(const StatMod& stat_mod) {
  static_assert(sizeof(save::StatMod) == sizeof(StatMod), "struct size mismatches");

  return reinterpret_cast<const save::StatMod*>(&stat_mod);
}

flatbuffers::Offset<save::EquipmentManager> SaveFile::Build(const EquipmentManager& em) {
  std::vector<flatbuffers::Offset<save::Equipment>> records;
  em.ForEach([&](const string& id, const Equipment& equipment) {
    ASSERT(id == equipment.GetId());
    records.push_back(Build(equipment));
  });
  return save::CreateEquipmentManager(builder_, builder_.CreateVector(records));
}

flatbuffers::Offset<save::Equipment> SaveFile::Build(const Equipment& equipment) {
  auto id_off = builder_.CreateString(equipment.GetId());
  auto type = static_cast<int>(equipment.GetType());
  auto va_off = Build(equipment.volatile_attribute());
  return save::CreateEquipment(builder_, id_off, type, va_off);
}

flatbuffers::Offset<save::VolatileAttributes> SaveFile::Build(const VolatileAttribute& va) {
  return save::CreateVolatileAttributes(builder_, Build(va.stat_modifier_list()));
}

flatbuffers::Offset<save::AttributeModifierList> SaveFile::Build(const StatModifierList& aml) {
  std::vector<flatbuffers::Offset<save::AttributeModifier>> list;
  aml.iterate([&](const StatModifier& am) {
    list.push_back(Build(am));
  });
  return save::CreateAttributeModifierList(builder_, builder_.CreateVector(list));
}

flatbuffers::Offset<save::AttributeModifier> SaveFile::Build(const StatModifier& am) {
  return save::CreateAttributeModifierDirect(builder_, am.id().c_str(), am.stat_id(), Build(am.turn()), Build(am.mod()));
}

void SaveFile::Deserialize() {
  std::ifstream file(path_.ToString(), std::ios::binary | std::ios::ate);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<char> buffer(size);
  file.read(buffer.data(), size);

  auto sce = save::GetScenario(buffer.data());
  auto val = sce->id();
  (void)val;
}

}  // namespace core
}  // namespace mengde
