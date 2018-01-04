#ifndef CONFIG_LOADER_H_
#define CONFIG_LOADER_H_

#include "util/common.h"
#include "util/game_env.h"
#include "resource_manager.h"

namespace lua {
  class LuaScript;
}

namespace mengde {
namespace core {

class LuaScript;
class EventEffect;

class ConfigLoader {
 public:
  ConfigLoader(const string&);
  ~ConfigLoader();
  const ResourceManagers& GetResources() const { return rc_; }
  const string& GetFirstStage() const { return first_stage_; }

 private:
  void ParseUnitClassesAndTerrains();
  void ParseMagics();
  void ParseEquipments();
  void ParseHeroTemplates();
  void ParseFirstStage();
  uint16_t StatStrToIdx(const string&);
  EventEffect* GenerateEventEffect(const string&, const string&, int);

 private:
  ::lua::LuaScript*       lua_config_;
  ResourceManagers rc_;
  string           first_stage_;
};

} // namespace mengde
} // namespace core

#endif // CONFIG_LOADER_H_
