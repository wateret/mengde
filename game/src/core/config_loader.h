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
  const ResourceManagers& GetResources() const { return rc_; }
  const string& GetFirstStage() const { return first_stage_; }

 private:
  void ParseUnitClassesAndTerrains();
  void ParseMagics();
  void ParseItems();
  void ParseHeroTemplates();
  void ParseFirstStage();
  uint16_t StatStrToIdx(const string&);
  EventEffect* GenerateEventEffect(const string&, int);

 private:
  LuaScript*       lua_config_;
  ResourceManagers rc_;
  string           first_stage_;
};

#endif // CONFIG_LOADER_H_
