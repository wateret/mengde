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
  return save::CreateResourceManagers(builder_, tm, hcm);
}

flatbuffers::Offset<save::HeroClassManager> SaveFile::Build(const HeroClassManager& hcm) {
  std::vector<flatbuffers::Offset<save::HeroClassRecord>> records;
  hcm.ForEach([&](const string& id, const HeroClass& hero_class) { records.push_back(Build(id, hero_class)); });
  return save::CreateHeroClassManager(builder_, builder_.CreateVector(records));
}

flatbuffers::Offset<save::HeroClassRecord> SaveFile::Build(const string& id, const HeroClass& hero_class) {
  auto id_off = builder_.CreateString(id);
  auto obj_off = Build(hero_class);
  return save::CreateHeroClassRecord(builder_, id_off, obj_off);
}

flatbuffers::Offset<save::HeroClass> SaveFile::Build(const HeroClass& hero_class) {
  auto attr_inl = Build(hero_class.stat_grade());
  auto attack_range = static_cast<int>(hero_class.attack_range_enum());
  auto move = hero_class.move();
  auto hp_inl = Build(hero_class.bni_hp());
  auto mp_inl = Build(hero_class.bni_mp());
  auto pi_off = hero_class.promotion_info() ? Build(*hero_class.promotion_info()) : 0;
  return save::CreateHeroClass(builder_, attr_inl, attack_range, move, hp_inl, mp_inl, pi_off);
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
