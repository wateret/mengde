#include "deserializer.h"
#include "scenario_generated.h"

namespace mengde {
namespace core {

unique_ptr<Scenario> Deserializer::Deserialize() {
  std::ifstream file(path_.ToString(), std::ios::binary | std::ios::ate);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<char> buffer(size);
  file.read(buffer.data(), size);

  auto sce = save::GetScenario(buffer.data());
  (void)sce;

  return nullptr;
}

}  // namespace core
}  // namespace mengde
