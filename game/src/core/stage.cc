#include "stage.h"

#include "assets.h"
#include "cmd.h"
#include "cmd_debug_printer.h"
#include "commander.h"
#include "core/path_tree.h"
#include "deployer.h"
#include "event_effect.h"
#include "exceptions.h"
#include "formulae.h"
#include "lua_callbacks.h"
#include "lua_game.h"
#include "magic.h"
#include "stage_unit_manager.h"
#include "user_interface.h"
#include "util/game_env.h"
#include "util/path.h"

namespace mengde {
namespace core {

Stage::Stage(const ResourceManagers& rc, const Assets* assets, const Path& stage_script_path)
    : rc_(rc),
      assets_{std::make_unique<Assets>(*assets)},
      lua_{},
      lua_config_{},
      lua_game_{new LuaGame{this}},
      lua_callbacks_{new LuaCallbacks},
      user_interface_{new UserInterface{this}},
      commander_{new Commander},
      deployer_{nullptr},
      map_{nullptr},
      stage_unit_manager_{new StageUnitManager},
      turn_{0},  // FIXME Currently intialization must be done after SetupLua
      status_(Status::kDeploying) {
  try {
    SetupLua(stage_script_path);

    turn_ = Turn{GetTurnLimit()};
    map_ = std::unique_ptr<Map>(CreateMap());

    // Run main function
    lua_["main"](*lua_game_);

    // Begin with on_deploy
    lua_callbacks_->on_deploy()(*lua_game_);

    deployer_ = std::unique_ptr<Deployer>(CreateDeployer());
  } catch (const std::exception& e) {
    LOG_ERROR(e.what());
    throw CoreException(std::string("Error while Stage script load : ") + e.what());
  }
}

Stage::~Stage() {
  // NOTE rc_ and assets_ are not deleted here
}

void Stage::SetupLua(const Path& stage_script_path) {
  // Run the user's script file
  lua_.open_libraries(sol::lib::base);

  const char* blacklist_lib_base[] = {"dofile", "load", "loadfile"};
  for (auto fn : blacklist_lib_base) {
    // TODO How to completely delete table entry instead of setting to nil
    lua_[fn] = sol::lua_nil;
  }

  lua_.script_file(stage_script_path.ToString());
  lua_config_ = lua_["gstage"];

  // clang-format off

  // Register game object
  lua_.new_usertype<LuaGame>("mengde",
      // NOTE no constructors since we do not want to create game object in Lua
      "appoint_hero", &LuaGame::AppointHero,
      "generate_own_unit", &LuaGame::GenerateOwnUnit,
      "generate_unit", &LuaGame::GenerateUnit,
      "obtain_equipment", &LuaGame::ObtainEquipment,
      "set_ai_mode", &LuaGame::SetAIMode,
      "get_num_enemies_alive", &LuaGame::GetNumEnemiesAlive,
      "get_num_owns_alive", &LuaGame::GetNumOwnsAlive,
      "get_unit_info", &LuaGame::GetUnitInfo,
      "get_unit_on_position", &LuaGame::GetUnitOnPosition,
      "get_terrain_on_position", &LuaGame::GetTerrainOnPosition,
      "set_on_deploy", &LuaGame::SetOnDeploy,
      "set_on_begin", &LuaGame::SetOnBegin,
      "set_on_victory", &LuaGame::SetOnVictory,
      "set_on_defeat", &LuaGame::SetOnDefeat,
      "set_end_condition", &LuaGame::SetEndCondition,
      "register_event", &LuaGame::RegisterEvent,
      "unregister_event", &LuaGame::UnregisterEvent,
      "cmd_move", &LuaGame::PushCmdMove,
      "cmd_speak", &LuaGame::PushCmdSpeak,
      "cmd_gain_exp", &LuaGame::PushCmdGainExp,
      "cmd_kill", &LuaGame::PushCmdKill
      );

  // clang-format on

  // Register enum values
  {
    auto enum_tbl = lua_["Enum"].get_or_create<sol::table>();
    {
      auto force_tbl = enum_tbl["force"].get_or_create<sol::table>();
      force_tbl["own"] = static_cast<int>(Force::kOwn);
      force_tbl["ally"] = static_cast<int>(Force::kAlly);
      force_tbl["enemy"] = static_cast<int>(Force::kEnemy);
    }
    {
      auto status_tbl = enum_tbl["status"].get_or_create<sol::table>();
      status_tbl["undecided"] = static_cast<int>(Status::kUndecided);
      status_tbl["defeat"] = static_cast<int>(Status::kDefeat);
      status_tbl["victory"] = static_cast<int>(Status::kVictory);
    }
  }
}

Map* Stage::CreateMap() {
  sol::table map_tbl = lua_config_["map"];
  auto size = map_tbl["size"];
  uint32_t cols = size[1];
  uint32_t rows = size[2];
  sol::table terrain_tbl = map_tbl["terrain"];
  std::vector<string> terrain;
  for (uint32_t i = 1, size = terrain_tbl.size(); i <= size; i++) {
    string row = terrain_tbl[i];
    terrain.push_back(row);
  }
  string file = map_tbl["file"];  // FIXME filename should be same as stage id + .bmp
  ASSERT(rows == terrain.size());
  for (auto e : terrain) {
    ASSERT(cols == e.size());
  }
  return new Map(user_interface(), terrain, file, rc_.terrain_manager);
}

Deployer* Stage::CreateDeployer() {
  sol::table deploy_tbl = lua_config_["deploy"];

  vector<DeployInfoUnselectable> unselectable_info_list;
  sol::table unselectables = deploy_tbl["unselectables"];
  for (uint32_t i = 1, size = unselectables.size(); i <= size; i++) {
    sol::table e = unselectables[i];
    string hero_id = e["hero"];
    int pos_x = e["position"][1];
    int pos_y = e["position"][2];
    Vec2D position(pos_x, pos_y);
    Hero* hero = assets_->LookupHero(hero_id);  // TODO Check if Hero exists in our assets
    unselectable_info_list.push_back({position, hero});
  }

  uint32_t num_required = deploy_tbl["num_required_selectables"];

  vector<DeployInfoSelectable> selectable_info_list;
  sol::table selectables = deploy_tbl["selectables"];
  for (uint32_t i = 1, size = selectables.size(); i <= size; i++) {
    sol::table e = selectables[i];
    int pos_x = e["position"][1];
    int pos_y = e["position"][2];
    Vec2D position(pos_x, pos_y);
    selectable_info_list.push_back({position});
  }
  return new Deployer(unselectable_info_list, selectable_info_list, num_required);
}

uint16_t Stage::GetTurnLimit() { return lua_config_["turn_limit"]; }

void Stage::ForEachUnit(std::function<void(Unit*)> fn) { stage_unit_manager_->ForEach(fn); }

void Stage::ForEachUnitConst(std::function<void(const Unit*)> fn) const { stage_unit_manager_->ForEachConst(fn); }

void Stage::MoveUnit(Unit* unit, Vec2D dst) {
  Vec2D src = unit->position();
  if (src == dst) return;
  map_->MoveUnit(src, dst);
  unit->position(dst);
}

void Stage::MoveUnit(const UId& uid, Vec2D dst) { MoveUnit(LookupUnit(uid), dst); }

void Stage::KillUnit(Unit* unit) {
  map_->RemoveUnit(unit->position());
  stage_unit_manager_->Kill(unit);
}

bool Stage::TryBasicAttack(Unit* unit_atk, Unit* unit_def) {
  return GenRandom(100) < formulae::ComputeBasicAttackAccuracy(unit_atk, unit_def);
}

bool Stage::TryMagic(Unit* unit_atk, Unit* unit_def) {
  return GenRandom(100) < formulae::ComputeMagicAccuracy(unit_atk, unit_def);
}

bool Stage::IsValidCoords(Vec2D c) const { return map_->IsValidCoords(c); }

Magic* Stage::LookupMagic(const std::string& id) { return rc_.magic_manager->Get(id); }

Unit* Stage::LookupUnit(const UId& id) { return stage_unit_manager_->Get(id); }

const Unit* Stage::LookupUnit(const UId& id) const { return stage_unit_manager_->Get(id); }

Equipment* Stage::LookupEquipment(const std::string& id) { return rc_.equipment_manager->Get(id); }

Force Stage::GetCurrentForce() const { return turn_.force(); }

bool Stage::EndForceTurn() {
  ForEachUnit([this](Unit* u) {
    if (this->IsCurrentTurn(u)) {
      u->ResetAction();
    }
  });

  bool next_turn = turn_.Next();

  ForEachUnit([this](Unit* u) {
    if (this->IsCurrentTurn(u)) {
      u->NextTurn();
      commander_->Push(u->RaiseEvent(event::GeneralEvent::kTurnBegin));
    }
  });

  if (next_turn) {
    // TODO do stuff when next turn begins (when actually the turn number increased)
  }

  return IsUserTurn();
}

bool Stage::IsUserTurn() const { return turn_.force() == Force::kOwn; }

bool Stage::IsAITurn() const { return !IsUserTurn(); }

bool Stage::IsCurrentTurn(Unit* unit) const { return unit->force() == turn_.force(); }

const Turn& Stage::GetTurn() const { return turn_; }

vector<Vec2D> Stage::FindMovablePos(Unit* unit) {
  std::unique_ptr<PathTree> path_tree{FindMovablePath(unit)};
  auto list = path_tree->GetNodeList();

  // Remove positions that another unit is present
  // TODO Enhance this inefficient algorithm O(M * N) where
  //      M is the number of res elements
  //      N is the number of units in this stage
  list.erase(std::remove_if(list.begin(), list.end(),
                            [&](const Vec2D& e) {
                              bool erase = false;
                              ForEachUnit([&](const Unit* u) {
                                erase = erase || (unit != u && !u->IsDead() && u->position() == e);
                              });
                              return erase;
                            }),
             list.end());

  return list;
}

PathTree* Stage::FindMovablePath(Unit* unit) { return map_->FindMovablePath(unit->uid()); }

bool Stage::HasNext() const { return commander_->HasNext(); }

const Cmd* Stage::GetNextCmdConst() const {
  ASSERT(HasNext());
  return commander_->GetNextCmdConst();
}

bool Stage::UnitInCell(Vec2D c) const { return map_->UnitInCell(c); }

const Unit* Stage::GetUnitInCell(Vec2D c) const {
  if (!map_->UnitInCell(c)) return nullptr;
  return LookupUnit(map_->GetUnitId(c));
}

const Cell* Stage::GetCell(Vec2D c) const { return map_->GetCell(c); }

void Stage::DoNext() {
  ASSERT(HasNext());
#ifdef DEBUG
  {
    const auto& cmdq = commander_->queue();
    CmdDebugPrinter printer{this};
    cmdq.Accept(printer);
    LOG_INFO("%s", printer.TakeDumped().c_str());
  }
#endif
  commander_->DoNext(this);
}

void Stage::Push(unique_ptr<Cmd> cmd) {
  ///  if (cmd == nullptr) return;
  commander_->Push(std::move(cmd));
}

bool Stage::CheckStatus() {
  if (status_ != Status::kUndecided) return false;
  uint32_t res = lua_callbacks_->end_condition()(*lua_game_);
  status_ = static_cast<Status>(res);
  return (status_ != Status::kUndecided);
}

uint32_t Stage::GetNumEnemiesAlive() {
  uint32_t count = 0;
  ForEachUnit([=, &count](Unit* u) {
    if (!u->IsDead() && u->force() == Force::kEnemy) {
      count++;
    }
  });
  return count;
}

uint32_t Stage::GetNumOwnsAlive() {
  uint32_t count = 0;
  ForEachUnit([=, &count](Unit* u) {
    if (!u->IsDead() && u->force() == Force::kOwn) {
      count++;
    }
  });
  return count;
}

void Stage::AppointHero(const string& id, uint16_t level) {
  LOG_INFO("Hero added to asset '%s' with Lv %d", id.c_str(), level);
  assets_->AddHero(std::make_unique<Hero>(rc_.hero_tpl_manager->Get(id), Level{level, 0}));
}

uint32_t Stage::GenerateOwnUnit(const string& id, Vec2D pos) {
  auto hero = assets_->LookupHero(id);
  return GenerateOwnUnit(hero, pos);
}

uint32_t Stage::GenerateOwnUnit(Hero* hero, Vec2D pos) {
  Unit* unit = new Unit(hero, Force::kOwn);
  auto uid = stage_unit_manager_->Deploy(unit);
  unit->position(pos);
  map_->PlaceUnit(uid, pos);
  ASSERT(unit->uid() == uid);
  return uid.Value();
}

uint32_t Stage::GenerateUnit(const string& id, uint16_t level, Force force, Vec2D pos) {
  HeroTemplate* hero_tpl = rc_.hero_tpl_manager->Get(id);
  Hero* hero = new Hero(hero_tpl, Level{level, 0});
  Unit* unit = new Unit(hero, force);
  auto uid = stage_unit_manager_->Deploy(unit);
  unit->position(pos);
  map_->PlaceUnit(uid, pos);
  ASSERT(unit->uid() == uid);
  return uid.Value();
}

void Stage::ObtainEquipment(const string& id, uint32_t amount) {
  Equipment* eq = rc_.equipment_manager->Get(id);
  assets_->AddEquipment(eq, amount);
}

void Stage::SetOnDeploy(const sol::function& fn) { lua_callbacks_->on_deploy(fn); }

void Stage::SetOnBegin(const sol::function& fn) { lua_callbacks_->on_begin(fn); }

void Stage::SetOnVictory(const sol::function& fn) { lua_callbacks_->on_victory(fn); }

void Stage::SetOnDefeat(const sol::function& fn) { lua_callbacks_->on_defeat(fn); }

void Stage::SetEndCondition(const sol::function& fn) { lua_callbacks_->end_condition(fn); }

uint32_t Stage::RegisterEvent(const sol::function& predicate, const sol::function& handler) {
  return lua_callbacks_->RegisterEvent(predicate, handler);
}

void Stage::UnregisterEvent(uint32_t id) { return lua_callbacks_->UnregisterEvent(id); }

void Stage::SetAIMode(const UId& uid, AIMode ai_mode) { stage_unit_manager_->SetAIMode(uid, ai_mode); }

void Stage::RunEvents() { return lua_callbacks_->RunEvents(*lua_game_); }

bool Stage::SubmitDeploy() {
  ASSERT(status_ == Status::kDeploying);
  if (status_ != Status::kDeploying) return true;

  if (!deployer_->IsReady()) return false;

  deployer_->ForEach([=](const DeployElement& e) {
    auto hero = assets_->LookupHero(e.hero->id());
    ASSERT(hero == e.hero);
    this->GenerateOwnUnit(hero, deployer_->position(hero));
  });

  status_ = Status::kUndecided;
  lua_callbacks_->on_begin()(*lua_game_);
  return true;
}

uint32_t Stage::AssignDeploy(const Hero* hero) { return deployer_->Assign(hero); }

uint32_t Stage::UnassignDeploy(const Hero* hero) { return deployer_->Unassign(hero); }

uint32_t Stage::FindDeploy(const Hero* hero) { return deployer_->Find(hero); }

const IAIUnit* Stage::GetAIUnit(const UId& uid) const { return stage_unit_manager_->GetAIUnit(uid); }

}  // namespace core
}  // namespace mengde
