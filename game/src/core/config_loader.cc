#include "config_loader.h"

#include "attack_range.h"
#include "attribute_modifier.h"
#include "equipment.h"
#include "event_effect.h"
#include "exceptions.h"
#include "hero.h"
#include "hero_class.h"
#include "magic.h"
#include "stat.h"
#include "terrain.h"
#include "util/common.h"
#include "util/path.h"

namespace mengde {
namespace core {

EventEffectLoader::EventEffectLoader() {
  gee_map_.emplace("action_done", event::GeneralEvent::kActionDone);
  gee_map_.emplace("turn_begin", event::GeneralEvent::kTurnBegin);
  ocee_map_.emplace("on_normal_attack", event::OnCmdEvent::kNormalAttack);
  ocee_map_.emplace("on_normal_attacked", event::OnCmdEvent::kNormalAttacked);
}

GeneralEventEffect* EventEffectLoader::CreateGeneralEventEffect(const sol::table& table) const {
  string str_effect = table["effect"];
  string str_event = table["event"];

  // Find Event Type
  auto found = gee_map_.find(str_event);
  ASSERT(found != gee_map_.end());
  event::GeneralEvent event = found->second;
  ASSERT(event != event::GeneralEvent::kNone);

  // Find Effect Type
  if (str_effect == "restore_hp") {
    auto mult = static_cast<int16_t>(table.get_or("multiplier", 0));
    auto add = static_cast<int16_t>(table.get_or("addend", 0));
    return new GEERestoreHp(event, {add, mult});
  }

  throw DataFormatException("Such GeneralEventEffect '" + str_effect + "' does not exist");
}

OnCmdEventEffect* EventEffectLoader::CreateOnCmdEventEffect(const sol::table& table) const {
  string str_effect = table["effect"];
  string str_event = table["event"];

  // Find Event Type
  auto found = ocee_map_.find(str_event);
  ASSERT(found != ocee_map_.end());
  event::OnCmdEvent event = found->second;
  ASSERT(event != event::OnCmdEvent::kNone);

  // Find Effect Type
  if (str_effect == "preemptive_attack") {
    return new OCEEPreemptiveAttack(event);
  } else if (str_effect == "enhance_basic_attack") {
    auto mult = static_cast<int16_t>(table.get_or("multiplier", 0));
    auto add = static_cast<int16_t>(table.get_or("addend", 0));
    return new OCEEEnhanceBasicAttack(event, CmdBasicAttack::Type::kActiveOrCounter, {add, mult});
  } else if (str_effect == "double_attack") {
    ASSERT(event == event::OnCmdEvent::kNormalAttack);  // TODO Change it to throw
    return new OCEEDoubleAttack{event};
  } else if (str_effect == "critical_attack") {
    ASSERT(event == event::OnCmdEvent::kNormalAttack);  // TODO Change it to throw
    return new OCEECriticalAttack{event};
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

ConfigLoader::ConfigLoader(const Path& filename) : lua_{}, rc_() {
  lua_.open_libraries(sol::lib::base /*, sol::lib::coroutine, sol::lib::string, sol::lib::io*/);

  Path path = GameEnv::GetInstance()->GetScenarioPath() / filename;

  try {
    if (!path.Exists()) throw CoreException("Config file '" + path.ToString() + "' does not exist.");
    lua_.script_file(path.ToString());
    ParseUnitClassesAndTerrains();
    ParseMagics();
    ParseEquipments();
    ParseHeroTemplates();
    ParseStages();
  } catch (const DataFormatException& e) {
    LOG_ERROR("%s", e.what());
    throw ConfigLoadException(e.what());
  }
  // TODO Handle sol2 execptions
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

Condition ConfigLoader::StringToCondition(const string& s) {
  if (s == "stunned") return Condition::kStunned;
  if (s == "poisoned") return Condition::kPoisoned;
  if (s == "rooted") return Condition::kRooted;
  UNREACHABLE("Invalid Condition string");
  return Condition::kNone;
}

void ConfigLoader::ParseUnitClassesAndTerrains() {
  auto grade_char_to_int = [](const char grade) -> int {
    if (grade == 'S') return 5;
    if (grade == 'A') return 4;
    if (grade == 'B') return 3;
    if (grade == 'C') return 2;
    if (grade == 'D') return 1;
    return 0;
  };

  rc_.hero_class_manager = new HeroClassManager();
  sol::table hero_classes = lua_["gconfig"]["hero_classes"];
  for (uint32_t i = 1, size = hero_classes.size(); i <= size; i++) {
    auto hero_class = hero_classes[i];
    string id = hero_class["id"];
    string grades = hero_class["attr_grades"];
    string range_s = hero_class["attack_range"];
    int move = hero_class["move"];
    auto promotion_info_table = hero_class["promotion"];  // optional
    int hp_base = hero_class["hp"][1];
    int hp_incr = hero_class["hp"][2];
    int mp_base = hero_class["mp"][1];
    int mp_incr = hero_class["mp"][2];

    Range::Type range = Range::StringToRange(range_s);
    boost::optional<PromotionInfo> promotion_info;
    if (promotion_info_table.valid()) {
      string id = promotion_info_table["id"];
      int level = promotion_info_table["level"];
      promotion_info = PromotionInfo{id, level};
    }

    Attribute stat_grades = {grade_char_to_int(grades[0]), grade_char_to_int(grades[1]), grade_char_to_int(grades[2]),
                             grade_char_to_int(grades[3]), grade_char_to_int(grades[4])};
    HeroClass* cla =
        new HeroClass(id, i - 1, stat_grades, range, move, {hp_base, hp_incr}, {mp_base, mp_incr}, promotion_info);
    this->rc_.hero_class_manager->Add(id, cla);
  }

  uint32_t class_count = rc_.hero_class_manager->GetNumElements();

  rc_.terrain_manager = new TerrainManager();
  vector<string> ids;
  vector<char> asciis;
  vector<char> cmaps;
  sol::table terrains = lua_["gconfig"]["terrains"];
  for (uint32_t i = 1, size = terrains.size(); i <= size; i++) {
    auto terrain = terrains[i];
    string id = terrain["id"];
    string ascii = terrain["char"];
    ids.push_back(id);
    asciis.push_back(ascii[0]);
  }

  uint32_t terrain_count = ids.size();

  sol::table terrain_movecost = lua_["gconfig"]["terrain_movecost"];
  vector<vector<int>> cost_list(terrain_movecost.size());
  for (uint32_t i = 1, size = terrain_movecost.size(); i <= size; i++) {
    sol::table row = terrain_movecost[i];
    for (uint32_t j = 1, size = row.size(); j <= size; j++) {
      int elem = row[j];
      cost_list[i - 1].push_back(elem);
    }
  }
  // Verify row and column size
  if (cost_list.size() != terrain_count) throw DataFormatException("Incorrect size of terrain_movecost");
  for (auto e : cost_list) {
    if (e.size() != class_count) throw DataFormatException("Incorrect size of terrain_movecost");
  }

  sol::table terrain_effect = lua_["gconfig"]["terrain_effect"];
  vector<vector<int>> effect_list(terrain_effect.size());
  for (uint32_t i = 1, size = terrain_effect.size(); i <= size; i++) {
    sol::table row = terrain_effect[i];
    for (uint32_t j = 1, size = row.size(); j <= size; j++) {
      int elem = row[j];
      effect_list[i - 1].push_back(elem);
    }
  }
  // Verify row and column size
  if (effect_list.size() != terrain_count) throw DataFormatException("Incorrect size of terrain_effect");
  for (auto e : effect_list) {
    if (e.size() != class_count) throw DataFormatException("Incorrect size of terrain_effect");
  }

  for (uint32_t i = 0; i < terrain_count; i++) {
    Terrain* t = new Terrain(ids[i], asciis[i], cost_list[i], effect_list[i]);
    rc_.terrain_manager->Add(string(1, asciis[i]), t);
  }
}

void ConfigLoader::ParseMagics() {
  rc_.magic_manager = new MagicManager();

  sol::table magics = lua_["gconfig"]["magics"];

  for (auto& magics_itr : magics) {
    sol::table magic_l = magics_itr.second;
    string id = magic_l["id"];
    string range_s = magic_l["range"];
    string target_s = magic_l["target"];
    auto mp = magic_l["mp"];
    Range::Type range = Range::StringToRange(range_s);
    bool target = (target_s == "enemy");

    auto magic = new Magic{id, range, target, mp};

    sol::table effects = magic_l["effects"];
    for (auto& effect_itr : effects) {
      sol::table effect = effect_itr.second;
      string type_str = effect["type"];
      MagicEffectType type = [](const string& s) {
        if (s == "hp") return MagicEffectType::kHP;
        if (s == "attr") return MagicEffectType::kStat;
        if (s == "condition") return MagicEffectType::kCondition;
        UNREACHABLE("Invalid magic type");
        return MagicEffectType::kNone;
      }(type_str);

      switch (type) {
        case MagicEffectType::kHP: {
          int32_t power = effect["power"];
          magic->AddEffect(std::make_unique<MagicEffectHP>(power));
          break;
        }
        case MagicEffectType::kStat: {
          string stat_s = effect["attr"];
          int16_t amount = effect["amount"];
          auto turns = TurnBased{static_cast<uint16_t>(effect["turns"])};
          auto stat_id = StatStrToIdx(stat_s);
          AttributeChange stat_mod{0, amount};
          magic->AddEffect(std::make_unique<MagicEffectStat>(AttributeModifier{id, stat_id, stat_mod, turns}));
          break;
        }
        case MagicEffectType::kCondition: {
          auto condition = StringToCondition(effect["condition"]);
          auto turns = TurnBased{static_cast<uint16_t>(effect["turns"])};
          magic->AddEffect(std::make_unique<MagicEffectCondition>(condition, turns));
          break;
        }
        case MagicEffectType::kNone:
        default:
          UNREACHABLE("Unknown type of magic");
          break;
      };
    }

    sol::table learnats = magic_l["learnat"];
    for (auto& learnats_itr : learnats) {
      sol::table learnat = learnats_itr.second;
      string uclass = learnat["class"];
      uint16_t level = learnat["level"];
      magic->AddLearnInfo(rc_.hero_class_manager->Get(uclass)->index(), level);
    }

    rc_.magic_manager->Add(id, magic);
  }
}

void ConfigLoader::ParseEquipments() {
  rc_.equipment_manager = new EquipmentManager();

  sol::table equipments = lua_["gconfig"]["equipments"];
  for (auto& itr : equipments) {
    sol::table equipment_tbl = itr.second;

    string id = equipment_tbl["id"];
    string type_s = equipment_tbl["type"];
    string equipable = equipment_tbl["equipable"];
    string desc = equipment_tbl["description"];
    Equipment::Type type = [](const string& type) {
      if (type == "weapon") return Equipment::Type::kWeapon;
      if (type == "armor") return Equipment::Type::kArmor;
      if (type == "aid") return Equipment::Type::kAid;
      return Equipment::Type::kNone;
    }(type_s);

    Equipment* equipment = new Equipment(id, type);

    sol::table effects = equipment_tbl["effects"];
    if (effects.valid()) {
      for (auto& itr : effects) {
        sol::table effect = itr.second;
        string event = effect["event"];
        auto ee_loader = EventEffectLoader::instance();
        if (ee_loader.IsGeneralEventEffect(event)) {
          equipment->AddGeneralEffect(ee_loader.CreateGeneralEventEffect(effect));
        } else if (ee_loader.IsOnCmdEventEffect(event)) {
          equipment->AddOnCmdEffect(ee_loader.CreateOnCmdEventEffect(effect));
        } else {
          throw DataFormatException("Such event '" + event + "' does not exist.");
        }
      }
    }

    sol::table modifiers = equipment_tbl["modifiers"];
    if (modifiers.valid()) {
      for (auto& itr : modifiers) {
        sol::table modifier_tbl = itr.second;
        string stat_s = modifier_tbl["attr"];
        int16_t addend = modifier_tbl.get_or("addend", 0);
        int16_t multiplier = modifier_tbl.get_or("multiplier", 0);
        AttributeModifier* mod = new AttributeModifier{id, StatStrToIdx(stat_s), {addend, multiplier}};
        equipment->AddModifier(mod);
      }
    }

    this->rc_.equipment_manager->Add(id, equipment);
  }
}

void ConfigLoader::ParseHeroTemplates() {
  rc_.hero_tpl_manager = new HeroTemplateManager();

  sol::table heroes = lua_["gconfig"]["heroes"];
  for (auto& itr : heroes) {
    sol::table hero_tbl = itr.second;

    string id = hero_tbl["id"];
    string uclass = hero_tbl["class"];
    sol::table attr_tbl = hero_tbl["attr"];
    const uint32_t num_attr = 5;
    if (attr_tbl.size() != num_attr) throw DataFormatException("Hero must have exactly 5 attribute members.");
    Attribute attr;
    for (uint32_t i = 1; i <= num_attr; i++) {
      attr[i - 1] = attr_tbl[i];
    }

    HeroTemplate* hero_tpl = new HeroTemplate(id, rc_.hero_class_manager->Get(uclass), attr);
    rc_.hero_tpl_manager->Add(id, hero_tpl);
  }
}

void ConfigLoader::ParseStages() {
  sol::table stages_tbl = lua_["gconfig"]["stages"];
  for (uint32_t i = 1, size = stages_tbl.size(); i <= size; i++) {
    string stage = stages_tbl[i];
    stages_.push_back(stage);
  }
}

}  // namespace core
}  // namespace mengde
