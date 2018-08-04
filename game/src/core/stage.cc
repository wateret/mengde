#include "stage.h"

#include "assets.h"
#include "cmd.h"
#include "commander.h"
#include "core/path_tree.h"
#include "deployer.h"
#include "event_effect.h"
#include "formulae.h"
#include "magic.h"
#include "stage_unit_manager.h"
#include "user_interface.h"
#include "util/game_env.h"
#include "util/path.h"

// XXX temporary include
#include "lua_api.h"

namespace mengde {
namespace core {

Stage::Stage(const ResourceManagers& rc, Assets* assets, const Path& stage_script_path)
    : rc_(rc),
      assets_(assets),  // FIXME Change this to clone the object as we need to rollback assets for some cases
      lua_(nullptr),
      lua_this_(this, "Game"),
      user_interface_(new UserInterface(this)),
      commander_(nullptr),
      deployer_(nullptr),
      map_(nullptr),
      stage_unit_manager_(nullptr),
      turn_(),
      status_(Status::kDeploying) {
  lua_ = CreateLua(stage_script_path);
  map_ = CreateMap();
  lua_->Call<void>(string("on_deploy"), lua_this_);
  deployer_ = CreateDeployer();

  commander_ = new Commander();
  stage_unit_manager_ = new StageUnitManager();
}

Stage::~Stage() {
  // NOTE rc_ and assets_ are not deleted here
  delete lua_;
  delete commander_;
  delete deployer_;
  delete map_;
  delete stage_unit_manager_;
  delete user_interface_;
}

lua::Lua* Stage::CreateLua(const Path& stage_script_path) {
  lua::Lua* lua = new lua::Lua();

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
  return new Map(user_interface_, terrain, file, rc_.terrain_manager);
}

Deployer* Stage::CreateDeployer() {
  ASSERT(lua_ != nullptr);

  vector<DeployInfoUnselectable> unselectable_info_list;
  lua_->ForEachTableEntry("gstage.deploy.unselectables", [=, &unselectable_info_list](lua::Lua* l, const string&) {
    vector<int> pos_vec = l->Get<vector<int>>("position");
    string hero_id = l->Get<string>("hero");
    Vec2D position(pos_vec[0], pos_vec[1]);
    Hero* hero = assets_->GetHero(hero_id);  // TODO Check if Hero exists in our assets
    unselectable_info_list.push_back({position, hero});
  });
  uint32_t num_required = lua_->Get<uint32_t>("gstage.deploy.num_required_selectables");
  vector<DeployInfoSelectable> selectable_info_list;
  lua_->ForEachTableEntry("gstage.deploy.selectables", [=, &selectable_info_list](lua::Lua* l, const string&) mutable {
    vector<int> pos_vec = l->Get<vector<int>>("position");
    Vec2D position(pos_vec[0], pos_vec[1]);
    selectable_info_list.push_back({position});
  });
  return new Deployer(unselectable_info_list, selectable_info_list, num_required);
}

void Stage::ForEachUnit(std::function<void(Unit*)> fn) { stage_unit_manager_->ForEach(fn); }

void Stage::ForEachUnitConst(std::function<void(const Unit*)> fn) const { stage_unit_manager_->ForEachConst(fn); }

void Stage::MoveUnit(Unit* unit, Vec2D dst) {
  Vec2D src = unit->GetPosition();
  if (src == dst) return;
  map_->MoveUnit(src, dst);
  unit->SetPosition(dst);
}

void Stage::MoveUnit(const UId& uid, Vec2D dst) { MoveUnit(GetUnit(uid), dst); }

void Stage::KillUnit(Unit* unit) {
  map_->RemoveUnit(unit->GetPosition());
  stage_unit_manager_->Kill(unit);
}

bool Stage::TryBasicAttack(Unit* unit_atk, Unit* unit_def) {
  return GenRandom(100) < Formulae::ComputeBasicAttackAccuracy(unit_atk, unit_def);
}

bool Stage::TryMagic(Unit* unit_atk, Unit* unit_def) {
  return GenRandom(100) < Formulae::ComputeMagicAccuracy(unit_atk, unit_def);
}

bool Stage::IsValidCoords(Vec2D c) { return map_->IsValidCoords(c); }

Magic* Stage::GetMagic(const std::string& id) { return rc_.magic_manager->Get(id); }

Unit* Stage::GetUnit(const UId& id) { return stage_unit_manager_->Get(id); }

const Unit* Stage::GetUnit(const UId& id) const {
  //  LOG_DEBUG("GetUnit UID : %u", id.Value());
  return stage_unit_manager_->Get(id);
}

/*
uint32_t Stage::GetUnitId(const Unit* unit) {
  // TODO The implementation is temoporary. Remove this method or write a safer implementation
  uint32_t id = 0;
  uint32_t idx = 0;
  ForEachUnit([&](Unit* u) {
    if (unit == u) {
      id = idx;
    }
    idx++;
  });
  return id;
}
*/

Equipment* Stage::GetEquipment(const std::string& id) { return rc_.equipment_manager->Get(id); }

Force Stage::GetCurrentForce() const { return turn_.GetForce(); }

bool Stage::EndForceTurn() {
  ForEachUnit([this](Unit* u) {
    if (this->IsCurrentTurn(u)) {
      u->ResetAction();
    }
  });

  bool next_turn = turn_.Next();

  ForEachUnit([this](Unit* u) {
    if (this->IsCurrentTurn(u)) {
      commander_->Push(u->RaiseEvent(event::GeneralEvent::kTurnBegin));
    }
  });

  if (next_turn) {
    // TODO do stuff when next turn begins (when actually the turn number increased)
  }

  return IsUserTurn();
}

bool Stage::IsUserTurn() const { return turn_.GetForce() == Force::kOwn; }

bool Stage::IsAITurn() const { return !IsUserTurn(); }

bool Stage::IsCurrentTurn(Unit* unit) const { return unit->GetForce() == turn_.GetForce(); }

uint16_t Stage::GetTurnCurrent() const { return turn_.GetCurrent(); }

uint16_t Stage::GetTurnLimit() const { return turn_.GetLimit(); }

vector<Vec2D> Stage::FindMovablePos(Unit* unit) {
  PathTree* path_tree = FindMovablePath(unit);
  return path_tree->GetNodeList();
}

PathTree* Stage::FindMovablePath(Unit* unit) { return map_->FindMovablePath(unit->GetUnitId()); }

bool Stage::HasNext() const { return commander_->HasNext(); }

const Cmd* Stage::GetNextCmdConst() const {
  ASSERT(HasNext());
  return commander_->GetNextCmdConst();
}

bool Stage::UnitInCell(Vec2D c) const { return map_->UnitInCell(c); }

const Unit* Stage::GetUnitInCell(Vec2D c) const {
  if (!map_->UnitInCell(c)) return nullptr;
  return GetUnit(map_->GetUnitId(c));
}

const Cell* Stage::GetCell(Vec2D c) const { return map_->GetCell(c); }

void Stage::DoNext() {
  ASSERT(HasNext());
#ifdef DEBUG
  commander_->DebugPrint(this);
#endif
  commander_->DoNext(this);
}

void Stage::Push(unique_ptr<Cmd> cmd) {
  ///  if (cmd == nullptr) return;
  commander_->Push(std::move(cmd));
}

bool Stage::CheckStatus() {
  if (status_ != Status::kUndecided) return false;
  uint32_t res = lua_->Call<uint32_t>("end_condition", lua_this_);
  status_ = static_cast<Status>(res);
  return (status_ != Status::kUndecided);
}

uint32_t Stage::GetNumEnemiesAlive() {
  uint32_t count = 0;
  ForEachUnit([=, &count](Unit* u) {
    if (!u->IsDead() && u->GetForce() == Force::kEnemy) {
      count++;
    }
  });
  return count;
}

uint32_t Stage::GetNumOwnsAlive() {
  uint32_t count = 0;
  ForEachUnit([=, &count](Unit* u) {
    if (!u->IsDead() && u->GetForce() == Force::kOwn) {
      count++;
    }
  });
  return count;
}

void Stage::AppointHero(const string& id, uint16_t level) {
  LOG_INFO("Hero added to asset '%s' with Lv %d", id.c_str(), level);
  Hero* hero = new Hero(rc_.hero_tpl_manager->Get(id), level);
  assets_->AddHero(hero);
}

uint32_t Stage::GenerateOwnUnit(const string& id, Vec2D pos) {
  const Hero* hero = assets_->GetHero(id);
  return GenerateOwnUnit(hero, pos);
}

uint32_t Stage::GenerateOwnUnit(const Hero* hero, Vec2D pos) {
  Unit* unit = new Unit(hero, Force::kOwn);
  auto uid = stage_unit_manager_->Deploy(unit);
  unit->SetPosition(pos);
  map_->PlaceUnit(uid, pos);
  ASSERT(unit->GetUnitId() == uid);
  return uid.Value();
}

uint32_t Stage::GenerateUnit(const string& id, uint16_t level, Force force, Vec2D pos) {
  HeroTemplate* hero_tpl = rc_.hero_tpl_manager->Get(id);
  Hero* hero = new Hero(hero_tpl, level);
  Unit* unit = new Unit(hero, force);
  auto uid = stage_unit_manager_->Deploy(unit);
  unit->SetPosition(pos);
  map_->PlaceUnit(uid, pos);
  ASSERT(unit->GetUnitId() == uid);
  return uid.Value();
}

void Stage::ObtainEquipment(const string& id, uint32_t amount) {
  Equipment* eq = rc_.equipment_manager->Get(id);
  assets_->AddEquipment(eq, amount);
}

bool Stage::SubmitDeploy() {
  ASSERT(status_ == Status::kDeploying);
  if (status_ != Status::kDeploying) return true;

  if (!deployer_->IsReady()) return false;

  deployer_->ForEach([=](const DeployElement& e) { this->GenerateOwnUnit(e.hero, deployer_->GetPosition(e.hero)); });

  status_ = Status::kUndecided;
  lua_->Call<void>(string("on_begin"), lua_this_);
  return true;
}

uint32_t Stage::AssignDeploy(const Hero* hero) { return deployer_->Assign(hero); }

uint32_t Stage::UnassignDeploy(const Hero* hero) { return deployer_->Unassign(hero); }

uint32_t Stage::FindDeploy(const Hero* hero) { return deployer_->Find(hero); }

}  // namespace core
}  // namespace mengde
