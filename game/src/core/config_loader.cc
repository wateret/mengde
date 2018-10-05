#include "config_loader.h"

#include "attack_range.h"
#include "equipment.h"
#include "event_effect.h"
#include "exceptions.h"
#include "hero.h"
#include "hero_class.h"
#include "luab/lua.h"
#include "magic.h"
#include "stat.h"
#include "stat_modifier.h"
#include "terrain.h"
#include "util/common.h"
#include "util/path.h"

namespace mengde {
namespace core {

EventEffectLoader::EventEffectLoader() {
  gee_map_.insert({"action_done", event::GeneralEvent::kActionDone});
  gee_map_.insert({"turn_begin", event::GeneralEvent::kTurnBegin});
  ocee_map_.insert({"on_normal_attack", event::OnCmdEvent::kNormalAttack});
  ocee_map_.insert({"on_normal_attacked", event::OnCmdEvent::kNormalAttacked});
}

GeneralEventEffect* EventEffectLoader::CreateGeneralEventEffect(const luab::Table& table) const {
  auto str_effect = table.Get<std::string>("effect");
  auto str_event = table.Get<std::string>("event");

  // Find Event Type
  auto found = gee_map_.find(str_event);
  ASSERT(found != gee_map_.end());
  event::GeneralEvent event = found->second;
  ASSERT(event != event::GeneralEvent::kNone);

  // Find Effect Type
  if (str_effect == "restore_hp") {
    auto mult = table.Get<int>("multiplier", 0);
    auto add = table.Get<int>("addend", 0);
    return new GEERestoreHp(event, mult, add);
  }

  throw DataFormatException("Such GeneralEventEffect '" + str_effect + "' does not exist");
}

OnCmdEventEffect* EventEffectLoader::CreateOnCmdEventEffect(const luab::Table& table) const {
  auto str_effect = table.Get<std::string>("effect");
  auto str_event = table.Get<std::string>("event");

  // Find Event Type
  auto found = ocee_map_.find(str_event);
  ASSERT(found != ocee_map_.end());
  event::OnCmdEvent event = found->second;
  ASSERT(event != event::OnCmdEvent::kNone);

  // Find Effect Type
  if (str_effect == "preemptive_attack") {
    return new OCEEPreemptiveAttack(event);
  } else if (str_effect == "enhance_basic_attack") {
    auto mult = table.Get<int>("multiplier", 0);
    auto add = table.Get<int>("addend", 0);
    return new OCEEEnhanceBasicAttack(event, mult, add);
  }

  throw DataFormatException("Such OnCmdEventEffect '" + str_effect + "' does not exist");
}

bool EventEffectLoader::IsGeneralEventEffect(const std::string& key) const {
  return gee_map_.find(key) != gee_map_.end();
}

bool EventEffectLoader::IsOnCmdEventEffect(const std::string& key) const {
  return ocee_map_.find(key) != ocee_map_.end();
}

const EventEffectLoader& EventEffectLoader::instance() {
  static EventEffectLoader inst;
  return inst;
}

ConfigLoader::ConfigLoader(const Path& filename) : lua_config_(nullptr), rc_() {
  Path path = GameEnv::GetInstance()->GetScenarioPath() / filename;
  lua_config_ = new ::luab::Lua();

  try {
    lua_config_->RunFile(path.ToString());
    ParseUnitClassesAndTerrains();
    ParseMagics();
    ParseEquipments();
    ParseHeroTemplates();
    ParseStages();
  } catch (const luab::UndeclaredVariableException& e) {
    LOG_ERROR("%s", e.what());
    throw ConfigLoadException(e.what());
  } catch (const luab::WrongTypeException& e) {
    LOG_ERROR("%s", e.what());
    throw ConfigLoadException(e.what());
  } catch (const luab::ScriptRuntimeException& e) {
    LOG_ERROR("%s", e.what());
    throw ConfigLoadException(e.what());
  } catch (const DataFormatException& e) {
    LOG_ERROR("%s", e.what());
    throw ConfigLoadException(e.what());
  }
}

ConfigLoader::~ConfigLoader() {
  // We do not delete data created by this class,
  // we assume that corresponding Stage object will do that.

  delete lua_config_;
}

uint16_t ConfigLoader::StatStrToIdx(const string& s) {
  if (s == "atk") return 0;
  if (s == "def") return 1;
  if (s == "dex") return 2;
  if (s == "itl") return 3;
  if (s == "mor") return 4;
  if (s == "") return -1;
  // UNREACHABLE("Invalid stat ID");
  return -1;
}

void ConfigLoader::ParseUnitClassesAndTerrains() {
  rc_.unit_class_manager = new UnitClassManager();
  int class_idx = 0;
  lua_config_->ForEachTableEntry("gconfig.unit_classes", [&](luab::Lua* l, const string&) {
    string id = l->Get<string>("id");
    string grades = l->Get<string>("stat_grades");
    string range_s = l->Get<string>("attack_range");
    int move = l->Get<int>("move");
    auto promotion_info_table = l->GetOpt<luab::Table>("promotion");
    vector<int> hp = l->Get<vector<int>>("hp");
    vector<int> mp = l->Get<vector<int>>("mp");

    Range::Type range = Range::StringToRange(range_s);
    auto GradeCharToInt = [](const char grade) -> int {
      if (grade == 'S') return 5;
      if (grade == 'A') return 4;
      if (grade == 'B') return 3;
      if (grade == 'C') return 2;
      if (grade == 'D') return 1;
      return 0;
    };

    boost::optional<PromotionInfo> promotion_info;
    if (!promotion_info_table.empty()) {
      auto id = promotion_info_table->Get<string>("id");
      auto level = promotion_info_table->Get<int>("level");
      promotion_info = PromotionInfo{id, level};
    }

    Attribute stat_grades = {GradeCharToInt(grades[0]), GradeCharToInt(grades[1]), GradeCharToInt(grades[2]),
                             GradeCharToInt(grades[3]), GradeCharToInt(grades[4])};
    HeroClass* cla = new HeroClass(id, class_idx++, stat_grades, (Range::Type)range, move, {hp[0], hp[1]},
                                   {mp[0], mp[1]}, promotion_info);
    this->rc_.unit_class_manager->Add(id, cla);
  });
  uint32_t class_count = rc_.unit_class_manager->GetNumElements();

  rc_.terrain_manager = new TerrainManager();
  vector<string> ids;
  vector<char> cmaps;
  lua_config_->ForEachTableEntry("gconfig.terrains", [=, &ids, &cmaps](luab::Lua* l, const string&) mutable {
    string id = l->Get<string>("id");
    char cmap = l->Get<string>("char")[0];
    ids.push_back(id);
    cmaps.push_back(cmap);
  });
  uint32_t terrain_count = ids.size();

  vector<vector<int>> cost_list = lua_config_->Get<vector<vector<int>>>("gconfig.terrain_movecost");

  // Check row and column size
  if (cost_list.size() != terrain_count) throw DataFormatException("Incorrect size of terrain_movecost");
  for (auto e : cost_list) {
    if (e.size() != class_count) throw DataFormatException("Incorrect size of terrain_movecost");
  }

  vector<vector<int>> effect_list = lua_config_->Get<vector<vector<int>>>("gconfig.terrain_effect");

  // Check row and column size
  if (effect_list.size() != terrain_count) throw DataFormatException("Incorrect size of terrain_effect");
  for (auto e : effect_list) {
    if (e.size() != class_count) throw DataFormatException("Incorrect size of terrain_effect");
  }

  for (uint32_t i = 0; i < terrain_count; i++) {
    Terrain* t = new Terrain(ids[i], cost_list[i], effect_list[i]);
    rc_.terrain_manager->Add(string(1, cmaps[i]), t);
  }
}

void ConfigLoader::ParseMagics() {
  rc_.magic_manager = new MagicManager();
  lua_config_->ForEachTableEntry("gconfig.magics", [this](luab::Lua* l, const string&) {
    string id = l->Get<string>("id");
    string range_s = l->Get<string>("range");
    string target_s = l->Get<string>("target");
    string type_s = l->Get<string>("type");
    int mp = l->Get<int>("mp");

    Range::Type range = Range::StringToRange(range_s);
    bool target = (target_s == "enemy");
    Magic::MagicType type = [](const string& s) -> Magic::MagicType {
      if (s == "deal") return Magic::MagicType::kMagicDeal;
      if (s == "heal") return Magic::MagicType::kMagicHeal;
      if (s == "stat_mod") return Magic::MagicType::kMagicStatMod;
      UNREACHABLE("Invalid magic type");
      return Magic::MagicType::kMagicNone;
    }(type_s);

    Magic* magic = nullptr;

    switch (type) {
      case Magic::MagicType::kMagicDeal:
      case Magic::MagicType::kMagicHeal: {
        int power = l->Get<int>("power");
        magic = new Magic(id, type, range, target, mp, power, 0, 0, 0);
        break;
      }
      case Magic::MagicType::kMagicStatMod: {
        string stat_s = l->Get<string>("stat");
        int amount = l->Get<int>("amount");
        int turns = l->Get<int>("turns");
        uint16_t stat = StatStrToIdx(stat_s);
        magic = new Magic(id, type, range, target, mp, 0, stat, amount, turns);
        break;
      }
      default:
        UNREACHABLE("Unknown type of magic");
        break;
    };

    l->ForEachTableEntry("learnat", [=, &magic](luab::Lua* l, const string&) {
      string uclass = l->Get<string>("class");
      uint16_t level = (uint16_t)l->Get<int>("level");
      magic->AddLearnInfo(rc_.unit_class_manager->Get(uclass)->index(), level);  // FIXME
    });
    rc_.magic_manager->Add(id, magic);
  });
}

void ConfigLoader::ParseEquipments() {
  rc_.equipment_manager = new EquipmentManager();
  lua_config_->ForEachTableEntry("gconfig.equipments", [this](luab::Lua* l, const string&) {
    string id = l->Get<string>("id");
    string type_s = l->Get<string>("type");
    string equipable = l->Get<string>("equipable");
    string desc = l->Get<string>("description");
    Equipment::Type type = [](const string& type) {
      if (type == "weapon") return Equipment::Type::kWeapon;
      if (type == "armor") return Equipment::Type::kArmor;
      if (type == "aid") return Equipment::Type::kAid;
      return Equipment::Type::kNone;
    }(type_s);

    Equipment* equipment = new Equipment(id, type);

    l->ForEachTableEntry("effects", [=, &equipment](luab::Lua* l, const string&) {
      auto table = l->Get<luab::Table>();
      auto event = table.Get<std::string>("event");
      auto ee_loader = EventEffectLoader::instance();
      if (ee_loader.IsGeneralEventEffect(event)) {
        equipment->AddGeneralEffect(ee_loader.CreateGeneralEventEffect(table));
      } else if (ee_loader.IsOnCmdEventEffect(event)) {
        equipment->AddOnCmdEffect(ee_loader.CreateOnCmdEventEffect(table));
      } else {
        throw DataFormatException("Such event '" + event + "' does not exist.");
      }
    });
    l->ForEachTableEntry("modifiers", [=, &equipment](luab::Lua* l, const string&) {
      string stat_s = l->Get<string>("stat");
      auto addend = l->GetOpt<int16_t>("addend");
      auto multiplier = l->GetOpt<int16_t>("multiplier");
      StatModifier* mod = new StatModifier{id, StatStrToIdx(stat_s), {addend, multiplier}};
      equipment->AddModifier(mod);
    });
    this->rc_.equipment_manager->Add(id, equipment);
  });
}

void ConfigLoader::ParseHeroTemplates() {
  rc_.hero_tpl_manager = new HeroTemplateManager();
  lua_config_->ForEachTableEntry("gconfig.heroes", [this](luab::Lua* l, const string&) {
    string id = l->Get<string>("id");
    string uclass = l->Get<string>("class");
    vector<int> statr = l->Get<vector<int>>("stat");
    Attribute stat = {statr[0], statr[1], statr[2], statr[3], statr[4]};
    HeroTemplate* hero_tpl = new HeroTemplate(id, rc_.unit_class_manager->Get(uclass), stat);
    rc_.hero_tpl_manager->Add(id, hero_tpl);
  });
}

void ConfigLoader::ParseStages() { stages_ = lua_config_->Get<vector<string>>("gconfig.stages"); }

}  // namespace core
}  // namespace mengde
