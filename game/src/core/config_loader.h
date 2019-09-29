#ifndef MENGDE_CORE_CONFIG_LOADER_H_
#define MENGDE_CORE_CONFIG_LOADER_H_

#include <sol.hpp>

#include "condition.h"
#include "resource_manager.h"
#include "util/common.h"
#include "util/game_env.h"

class Path;

namespace mengde {
namespace core {

class GeneralEventEffect;
class OnCmdEventEffect;

class EventEffectLoader {
 public:
  using OCEEGenerator = std::function<OnCmdEventEffect*(event::OnCmdEvent event, const sol::table& table)>;
  using GEEGenerator = std::function<GeneralEventEffect*(event::GeneralEvent event, const sol::table& table)>;

 public:
  static const EventEffectLoader& instance();

 public:
  ~EventEffectLoader() = default;

  GeneralEventEffect* CreateGeneralEventEffect(const sol::table&) const;
  OnCmdEventEffect* CreateOnCmdEventEffect(const sol::table&) const;

  bool IsGeneralEventEffect(const std::string& key) const;
  bool IsOnCmdEventEffect(const std::string& key) const;

 private:
  EventEffectLoader();

  static GeneralEventEffect* CreateGEERestoreHp(event::GeneralEvent event, const sol::table& table);
  static OnCmdEventEffect* CreateOCEEPreemptiveAttack(event::OnCmdEvent event, const sol::table& table);
  static OnCmdEventEffect* CreateOCEEEnhanceBasicAttack(event::OnCmdEvent event, const sol::table& table);
  static OnCmdEventEffect* CreateOCEEDoubleAttack(event::OnCmdEvent event, const sol::table& table);
  static OnCmdEventEffect* CreateOCEECriticalAttack(event::OnCmdEvent event, const sol::table& table);
  static OnCmdEventEffect* CreateOCEECounterCounterAttack(event::OnCmdEvent event, const sol::table& table);
  static OnCmdEventEffect* CreateOCEEReflectAttack(event::OnCmdEvent event, const sol::table& table);

 private:
  std::unordered_map<std::string, event::GeneralEvent> gee_map_;
  std::unordered_map<std::string, event::OnCmdEvent> ocee_map_;
  std::unordered_map<std::string, GEEGenerator> gee_gen_map_;
  std::unordered_map<std::string, OCEEGenerator> ocee_gen_map_;
};

class ConfigLoader {
 public:
  ConfigLoader(const Path&);
  const ResourceManagers& GetResources() const { return rc_; }
  const vector<string>& GetStages() const { return stages_; }

 private:
  void ParseUnitClassesAndTerrains();
  void ParseMagics();
  void ParseEquipments();
  void ParseHeroTemplates();
  void ParseStages();
  uint16_t StatStrToIdx(const string&);
  Condition StringToCondition(const string& s);

 private:
  sol::state lua_;
  ResourceManagers rc_;
  vector<string> stages_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_CONFIG_LOADER_H_
