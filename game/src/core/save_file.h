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
  void Serialize(const Scenario* scenario);

 private:
  flatbuffers::Offset<save::Scenario> Build(flatbuffers::FlatBufferBuilder& builder, const Scenario* scenario);
//  void Build(const ResourceManagers* rm);
//  void Build(const TerrainManager* tm);

 public:
  void Deserialize();

 private:
  Path path_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_SAVE_FILE_H_
