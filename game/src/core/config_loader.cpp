#include "config_loader.h"

#include "util/common.h"
#include "util/path.h"
#include "unit_class.h"
#include "attack_range.h"
#include "magic.h"
#include "stat.h"
#include "equipment.h"
#include "stat_modifier.h"
#include "event_effect.h"
#include "lua/lua.h"
#include "terrain.h"
#include "hero.h"

namespace mengde {
namespace core {

ConfigLoader::ConfigLoader(const Path& filename)
    : lua_config_(nullptr),
      rc_() {
  Path path = GameEnv::GetInstance()->GetScenarioPath() / filename;
  lua_config_ = new ::lua::Lua();
  lua_config_->Run(path.ToString());

  ParseUnitClassesAndTerrains();
  ParseMagics();
  ParseEquipments();
  ParseHeroTemplates();
  ParseStages();
}

ConfigLoader::~ConfigLoader() {
  // We do not delete data created by this class,
  // we assume that corresponding Game object will do that.

  delete lua_config_;
}

GeneralEventEffect* ConfigLoader::GenerateGeneralEventEffect(const string& type, const string& event, int amount) {
  event::GeneralEvent event_type = event::GeneralEvent::kNone;
  if (event == "on_action_done") {
    event_type = event::GeneralEvent::kActionDone;
  }
  else if (event == "on_turn_begin") {
    event_type = event::GeneralEvent::kTurnBegin;
  }
  ASSERT(event_type != event::GeneralEvent::kNone);

  if (type == "restore_hp") {
    return new GEERestoreHp(event_type, amount, 0);
  }
  UNREACHABLE("Unknown ID");
  return nullptr;
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
  lua_config_->ForEachTableEntry("gconf.unit_classes", [&] (lua::Lua* l, const string&) {
    string id      = l->Get<string>("id");
    int promotions = l->Get<int>("promotions");
    string grades  = l->Get<string>("stat_grades");
    string range_s = l->Get<string>("attack_range");
    int move       = l->Get<int>("move");
    vector<int> hp = l->Get<vector<int>>("hp");
    vector<int> mp = l->Get<vector<int>>("mp");

    Range::Type range = Range::StringToRange(range_s);
    auto GradeCharToInt  = [] (const char grade) -> int {
      if (grade == 'S') return 5;
      if (grade == 'A') return 4;
      if (grade == 'B') return 3;
      if (grade == 'C') return 2;
      if (grade == 'D') return 1;
      return 0;
    };
    Attribute stat_grades = {GradeCharToInt(grades[0]),
                        GradeCharToInt(grades[1]),
                        GradeCharToInt(grades[2]),
                        GradeCharToInt(grades[3]),
                        GradeCharToInt(grades[4])};
    UnitClass* cla = new UnitClass(id,
                                   class_idx++,
                                   promotions,
                                   stat_grades,
                                   (Range::Type) range,
                                   move,
                                   {hp[0], hp[1]},
                                   {mp[0], mp[1]});
    this->rc_.unit_class_manager->Add(id, cla);
  });
  uint32_t class_count = rc_.unit_class_manager->GetNumElements();

  rc_.terrain_manager = new TerrainManager();
  vector<string> ids;
  vector<char> cmaps;
  lua_config_->ForEachTableEntry("gconf.terrains", [=, &ids, &cmaps] (lua::Lua* l, const string&) mutable {
    string id = l->Get<string>("id");
    char cmap = l->Get<string>("char")[0];
    ids.push_back(id);
    cmaps.push_back(cmap);
  });
  uint32_t terrain_count = ids.size();

  vector< vector<int> > cost_list = lua_config_->Get<vector<vector<int>>>("gconf.terrain_movecost");

  // Check row and column size
  if (cost_list.size() != terrain_count)
    throw "Incorrect size of terrain_movecost";
  for (auto e : cost_list) {
    if (e.size() != class_count) throw "Incorrect size of terrain_movecost";
  }

  vector< vector<int> > effect_list = lua_config_->Get<vector<vector<int>>>("gconf.terrain_effect");

  // Check row and column size
  if (effect_list.size() != terrain_count)
    throw "Incorrect size of terrain_effect";
  for (auto e : effect_list) {
    if (e.size() != class_count) throw "Incorrect size of terrain_effect";
  }

  for (uint32_t i = 0; i < terrain_count; i++) {
    Terrain* t = new Terrain(ids[i], cost_list[i], effect_list[i]);
    rc_.terrain_manager->Add(string(1, cmaps[i]), t);
  }
}

void ConfigLoader::ParseMagics() {
  rc_.magic_manager = new MagicManager();
  lua_config_->ForEachTableEntry("gconf.magics", [this] (lua::Lua* l, const string&) {
    string id       = l->Get<string>("id");
    string range_s  = l->Get<string>("range");
    string target_s = l->Get<string>("target");
    string type_s   = l->Get<string>("type");
    int    mp       = l->Get<int>("mp");

    Range::Type range = Range::StringToRange(range_s);
    bool target = (target_s == "enemy");
    Magic::MagicType type = [] (const string& s) -> Magic::MagicType {
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
        string   stat_s = l->Get<string>("stat");
        int      amount = l->Get<int>("amount");
        int      turns  = l->Get<int>("turns");
        uint16_t stat   = StatStrToIdx(stat_s);
        magic = new Magic(id, type, range, target, mp, 0, stat, amount, turns);
        break;
      }
      default:
        UNREACHABLE("Unknown type of magic");
        break;
    };

    l->ForEachTableEntry("learnat", [=, &magic] (lua::Lua* l, const string&) {
      string uclass = l->Get<string>("class");
      uint16_t level = (uint16_t) l->Get<int>("level");
      magic->AddLearnInfo(rc_.unit_class_manager->Get(uclass)->GetIndex(), level); // FIXME
    });
    rc_.magic_manager->Add(id, magic);
  });
}

void ConfigLoader::ParseEquipments() {
  rc_.equipment_manager = new EquipmentManager();
  lua_config_->ForEachTableEntry("gconf.equipments", [this] (lua::Lua* l, const string&) {
    string id        = l->Get<string>("id");
    string type_s    = l->Get<string>("type");
    string equipable = l->Get<string>("equipable");
    string desc      = l->Get<string>("description");
    Equipment::Type type = [] (const string& type) {
      if (type == "weapon") return Equipment::Type::kWeapon;
      if (type == "armor") return Equipment::Type::kArmor;
      if (type == "aid") return Equipment::Type::kAid;
      return Equipment::Type::kNone;
    }(type_s);

    Equipment* equipment = new Equipment(id, type);

    l->ForEachTableEntry("effects", [=, &equipment] (lua::Lua* l, const string&) {
      string type = l->Get<string>("type");
      string event = l->Get<string>("event");
      // XXX Fix generation of EventEffect : gets different by type
      int amount = l->Get<int>("ratio");
      GeneralEventEffect* effect = GenerateGeneralEventEffect(type, event, amount);
      equipment->AddGeneralEffect(effect);
    });
    l->ForEachTableEntry("modifiers", [=, &equipment] (lua::Lua* l, const string&) {
      string   stat_s     = l->Get<string>("stat");
      uint16_t addend     = l->GetOpt<uint16_t>("addend");
      uint16_t multiplier = l->GetOpt<uint16_t>("multiplier");
      StatModifier* mod = new StatModifier(id, StatStrToIdx(stat_s), addend, multiplier);
      equipment->AddModifier(mod);
    });
    this->rc_.equipment_manager->Add(id, equipment);
  });
}

void ConfigLoader::ParseHeroTemplates() {
  rc_.hero_tpl_manager = new HeroTemplateManager();
  lua_config_->ForEachTableEntry("gconf.heroes", [this] (lua::Lua* l, const string&) {
    string id         = l->Get<string>("id");
    string uclass     = l->Get<string>("class");
    vector<int> statr = l->Get<vector<int>>("stat");
    string model      = l->GetOpt<string>("model");
    if (model == "nil") {
      model = "infantry-1-red"; // XXX hardcoded. Make this to find default model
    }
    Attribute stat = {statr[0], statr[1], statr[2], statr[3], statr[4]};
    HeroTemplate* hero_tpl = new HeroTemplate(id,
                                              model,
                                              rc_.unit_class_manager->Get(uclass),
                                              stat);
    rc_.hero_tpl_manager->Add(id, hero_tpl);
  });
}

void ConfigLoader::ParseStages() {
  stages_ = lua_config_->Get<vector<string>>("gconf.stages");
}

} // namespace core
} // namespace mengde
