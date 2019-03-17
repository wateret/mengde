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
  auto terrain_manager = Build(*rm.terrain_manager);
  return save::CreateResourceManagers(builder_, terrain_manager);
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
