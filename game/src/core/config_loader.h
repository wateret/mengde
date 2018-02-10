#ifndef CONFIG_LOADER_H_
#define CONFIG_LOADER_H_

#include "util/common.h"
#include "util/game_env.h"
#include "resource_manager.h"

class Path;

namespace lua {
  class Lua;
  class Table;
}

namespace mengde {
namespace core {

class GeneralEventEffect;

class EventEffectLoader {
 public:
  static const EventEffectLoader& instance();

 public:
  ~EventEffectLoader() = default;

  GeneralEventEffect* CreateGeneralEventEffect(const lua::Table*) const;
  OnCmdEventEffect* CreateOnCmdEventEffect(const lua::Table*) const;
  bool IsGeneralEventEffect(const std::string& key) const;
  bool IsOnCmdEventEffect(const std::string& key) const;

 private:
  EventEffectLoader();

 private:
  std::unordered_map<std::string, event::GeneralEvent> gee_map_;
  std::unordered_map<std::string, event::OnCmdEvent> ocee_map_;
};

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

 private:
  ::lua::Lua* lua_config_;
  ResourceManagers  rc_;
  vector<string> stages_;
};

} // namespace mengde
} // namespace core

#endif // CONFIG_LOADER_H_
