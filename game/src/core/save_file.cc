#include "save_file.h"

#include <fstream>

namespace mengde {
namespace core {

SaveFile::SaveFile(const Path& path) : path_{path} {}

void SaveFile::Serialize(const int* scenario) {
  flatbuffers::FlatBufferBuilder builder(1024);
  save::ScenarioBuilder sce_builder(builder);
  sce_builder.add_test_value(*scenario);
  auto sce = sce_builder.Finish();
  builder.Finish(sce);

  uint8_t* buffer = builder.GetBufferPointer();
  uint32_t size = builder.GetSize();

  std::ofstream file(path_.ToString(), std::ios::binary);
  file.write(reinterpret_cast<const char*>(buffer), size);
}

int* SaveFile::Deserialize() {
  std::ifstream file(path_.ToString(), std::ios::binary | std::ios::ate);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<char> buffer(size);
  file.read(buffer.data(), size);

  auto sce = save::GetScenario(buffer.data());
  auto val = sce->test_value();

  return new int{val};
}

}  // namespace core
}  // namespace mengde
