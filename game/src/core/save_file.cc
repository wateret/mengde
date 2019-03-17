#include "save_file.h"

#include <fstream>

#include "scenario.h"

namespace mengde {
namespace core {

SaveFile::SaveFile(const Path& path) : path_{path} {}

void SaveFile::Serialize(const Scenario* scenario) {
  flatbuffers::FlatBufferBuilder builder(1024);

  auto sce = Build(builder, scenario);

  builder.Finish(sce);

  uint8_t* buffer = builder.GetBufferPointer();
  uint32_t size = builder.GetSize();

  std::ofstream file(path_.ToString(), std::ios::binary);
  file.write(reinterpret_cast<const char*>(buffer), size);
}

flatbuffers::Offset<save::Scenario> SaveFile::Build(flatbuffers::FlatBufferBuilder& builder, const Scenario* scenario) {
  auto id_off = builder.CreateString(scenario->id());

  const auto& stage_id_list = scenario->stage_id_list();
  std::vector<flatbuffers::Offset<flatbuffers::String>> string_offsets;
  std::transform(stage_id_list.begin(), stage_id_list.end(),
                 std::back_inserter(string_offsets), [&](const std::string& s) {
        return builder.CreateString(s.c_str());
      });
  auto string_offset_vec = builder.CreateVector(string_offsets);

  save::ScenarioBuilder sce_builder(builder);
  sce_builder.add_id(id_off);
  sce_builder.add_stage_id_list(string_offset_vec);
  sce_builder.add_stage_no(scenario->stage_no());

  return sce_builder.Finish();
}

//void SaveFile::Build(const ResourceManagers* rm) {
//  Serialize(rm->terrain_manager);
//}
//
//void SaveFile::Build(const TerrainManager* tm) {
//}

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
