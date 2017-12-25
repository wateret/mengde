#ifndef CONFIG_LOADER_H_
#define CONFIG_LOADER_H_

#include "common.h"
#include "util/game_env.h"
#include "resource_manager.h"

class LuaScript;
class EventEffect;

class ConfigLoader {
 public:
  ConfigLoader(const string&);
  ~ConfigLoader();
  ResourceManagers GetResources() { return rc_; }

 private:
  void ParseUnitClassesAndTerrains();
  void ParseMagics();
  void ParseItems();
  void ParseHeroTemplates();
  uint16_t StatStrToIdx(const string&);
  EventEffect* GenerateEventEffect(const string&, int);

 private:
  LuaScript*       lua_config_;
  ResourceManagers rc_;
};

#endif // CONFIG_LOADER_H_
