#include "stage.h"

#include "assets.h"
#include "cmd.h"
#include "cmd_debug_printer.h"
#include "commander.h"
#include "core/path_tree.h"
#include "deployer.h"
#include "event_effect.h"
#include "formulae.h"
#include "lua_callbacks.h"
#include "luab/ref.h"
#include "magic.h"
#include "stage_unit_manager.h"
#include "user_interface.h"
#include "util/game_env.h"
#include "util/path.h"

// XXX temporary include
#include "lua_api.h"

namespace mengde {
namespace core {

Stage::Stage(const ResourceManagers& rc, const Assets* assets, const Path& stage_script_path)
    : rc_(rc),
      assets_{std::make_unique<Assets>(*assets)},
      lua_this_(this, "Game"),
      lua_{CreateLua(stage_script_path)},
      lua_callbacks_{new LuaCallbacks{lua_.get()}},
      user_interface_{new UserInterface{this}},
      commander_{new Commander},
      deployer_{nullptr},
      map_{nullptr},
      stage_unit_manager_{new StageUnitManager},
      turn_{GetTurnLimit()},
      status_(Status::kDeploying) {
  map_ = std::unique_ptr<Map>(CreateMap());

  // Run main function
  lua_->Call<void>(string{"main"}, lua_this_);

  lua_->Call<void>(lua_callbacks_->on_deploy(), lua_this_);
  deployer_ = std::unique_ptr<Deployer>(CreateDeployer());
}

Stage::~Stage() {
  // NOTE rc_ and assets_ are not deleted here
}

luab::Lua* Stage::CreateLua(const Path& stage_script_path) {
  luab::Lua* lua = new luab::Lua();

#define GAME_PREFIX "Game_"

  // Register API functions
  {
#define MACRO_LUA_GAME(cname, luaname) lua->Register(GAME_PREFIX #luaname, Game_##cname);
#include "lua_api_game.h.inc"
#undef MACRO_LUA_GAME
  }

  // Register OO-style pseudo class for Lua
  {
    lua->RegisterClass(lua_this_.name());

#define MACRO_LUA_GAME(cname, luaname) lua->RegisterMethod(lua_this_.name(), string{#luaname});
#include "lua_api_game.h.inc"
#undef MACRO_LUA_GAME
  }

#undef GAME_PREFIX

  // Register enum values
  {
#define ENUM_PREFIX "Enum."
    lua->Set(ENUM_PREFIX "force.own", (int)Force::kOwn);
    lua->Set(ENUM_PREFIX "force.ally", (int)Force::kAlly);
    lua->Set(ENUM_PREFIX "force.enemy", (int)Force::kEnemy);
    lua->Set(ENUM_PREFIX "status.undecided", (int)Status::kUndecided);
    lua->Set(ENUM_PREFIX "status.defeat", (int)Status::kDefeat);
    lua->Set(ENUM_PREFIX "status.victory", (int)Status::kVictory);
#undef GAME_PREFIX
  }

  // Run the main script
  lua->RunFile(stage_script_path.ToString());

  return lua;
}

Map* Stage::CreateMap() {
  ASSERT(lua_ != nullptr);

  auto size = lua_->Get<vector<uint32_t>>("gstage.map.size");
  uint32_t cols = size[0];
  uint32_t rows = size[1];
  auto terrain = lua_->Get<vector<string>>("gstage.map.terrain");
  string file = lua_->Get<string>("gstage.map.file");  // FIXME filename should be same as stage id + .bmp
  ASSERT(rows == terrain.size());
  for (auto e : terrain) {
    ASSERT(cols == e.size());
  }
  return new Map(user_interface(), terrain, file, rc_.terrain_manager);
}

Deployer* Stage::CreateDeployer() {
  ASSERT(lua_ != nullptr);

  vector<DeployInfoUnselectable> unselectable_info_list;
  lua_->ForEachTableEntry("gstage.deploy.unselectables", [=, &unselectable_info_list](luab::Lua* l, const string&) {
    vector<int> pos_vec = l->Get<vector<int>>("position");
    string hero_id = l->Get<string>("hero");
    Vec2D position(pos_vec[0], pos_vec[1]);
    Hero* hero = assets_->LookupHero(hero_id);  // TODO Check if Hero exists in our assets
    unselectable_info_list.push_back({position, hero});
  });
  uint32_t num_required = lua_->Get<uint32_t>("gstage.deploy.num_required_selectables");
  vector<DeployInfoSelectable> selectable_info_list;
  lua_->ForEachTableEntry("gstage.deploy.selectables", [=, &selectable_info_list](luab::Lua* l, const string&) mutable {
    vector<int> pos_vec = l->Get<vector<int>>("position");
    Vec2D position(pos_vec[0], pos_vec[1]);
    selectable_info_list.push_back({position});
  });
  return new Deployer(unselectable_info_list, selectable_info_list, num_required);
}

uint16_t Stage::GetTurnLimit() { return lua_->Get<uint16_t>("gstage.turn_limit"); }

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
  return GenRandom(100) < Formulae::ComputeBasicAttackAccuracy(unit_atk, unit_def);
}

bool Stage::TryMagic(Unit* unit_atk, Unit* unit_def) {
  return GenRandom(100) < Formulae::ComputeMagicAccuracy(unit_atk, unit_def);
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
  uint32_t res = lua_->Call<uint32_t>(lua_callbacks_->end_condition(), lua_this_);
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
  assets_->AddHero(std::make_unique<Hero>(rc_.hero_tpl_manager->Get(id), level));
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
  Hero* hero = new Hero(hero_tpl, level);
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

void Stage::SetOnDeploy(const luab::Ref& ref) { lua_callbacks_->on_deploy(ref); }

void Stage::SetOnBegin(const luab::Ref& ref) { lua_callbacks_->on_begin(ref); }

void Stage::SetOnVictory(const luab::Ref& ref) { lua_callbacks_->on_victory(ref); }

void Stage::SetOnDefeat(const luab::Ref& ref) { lua_callbacks_->on_defeat(ref); }

void Stage::SetEndCondition(const luab::Ref& ref) { lua_callbacks_->end_condition(ref); }

uint32_t Stage::RegisterEvent(const luab::Ref& condition, const luab::Ref& handler) {
  return lua_callbacks_->RegisterEvent(condition, handler);
}

void Stage::UnregisterEvent(uint32_t id) { return lua_callbacks_->UnregisterEvent(id); }

void Stage::SetAIMode(const UId& uid, AIMode ai_mode) { stage_unit_manager_->SetAIMode(uid, ai_mode); }

void Stage::RunEvents() { return lua_callbacks_->RunEvents(lua_this()); }

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
  lua_->Call<void>(lua_callbacks_->on_begin(), lua_this_);
  return true;
}

uint32_t Stage::AssignDeploy(const Hero* hero) { return deployer_->Assign(hero); }

uint32_t Stage::UnassignDeploy(const Hero* hero) { return deployer_->Unassign(hero); }

uint32_t Stage::FindDeploy(const Hero* hero) { return deployer_->Find(hero); }

}  // namespace core
}  // namespace mengde
