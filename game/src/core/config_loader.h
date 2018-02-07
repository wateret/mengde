#ifndef CONFIG_LOADER_H_
#define CONFIG_LOADER_H_

#include "util/common.h"
#include "util/game_env.h"
#include "resource_manager.h"

class Path;

namespace lua {
  class Olua;
}

namespace mengde {
namespace core {

class EventEffect;

class ConfigLoader {
 public:
  ConfigLoader(const Path&);
  ~ConfigLoader();
  const ResourceManagers& GetResources() const { return rc_; }
  const string& GetFirstStage() const { return stages_[0]; }

 private:
  void ParseUnitClassesAndTerrains();
  void ParseMagics();
  void ParseEquipments();
  void ParseHeroTemplates();
  void ParseStages();
  uint16_t StatStrToIdx(const string&);
  GeneralEventEffect* GenerateGeneralEventEffect(const string&, const string&, int);

 private:
  ::lua::Olua* lua_config_;
  ResourceManagers  rc_;
  vector<string> stages_;
};

} // namespace mengde
} // namespace core

#endif // CONFIG_LOADER_H_
