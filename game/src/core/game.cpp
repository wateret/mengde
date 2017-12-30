#include "game.h"
#include "assets.h"
#include "deployer.h"
#include "magic.h"
#include "formulae.h"
#include "cmd.h"
#include "util/game_env.h"
#include "util/path_tree.h"
#include "lua/lua_script.h"

// XXX temporary include
#include "event_effect.h"
#include "lua_game.h"

Game::Game(const ResourceManagers& rc, Assets* assets, const string& stage_script_path)
    : rc_(rc),
      assets_(assets),
      lua_script_(nullptr),
      commander_(),
      deployer_(nullptr),
      map_(nullptr),
      units_(),
      dead_units_(),
      turn_(),
      status_(Status::kDeploying) {
  InitLua(stage_script_path);

  { // Initalize map data
    vector<uint32_t> size = lua_script_->GetVector<uint32_t>("$gdata.map.size");
    uint32_t cols = size[0];
    uint32_t rows = size[1];
    vector<string> terrain = lua_script_->GetVector<string>("$gdata.map.terrain");
    string file = lua_script_->Get<string>("$gdata.map.file");
    ASSERT(rows == terrain.size());
    for (auto e : terrain) {
      ASSERT(cols == e.size());
    }
    map_ = new Map(terrain, file, rc_.terrain_manager);
  }

  { // Initialize Deployment
    lua_script_->Call<void>("$on_deploy");

    vector<Vec2D> position_list;
    lua_script_->ForEachTableEntry("$gdata.deploy", [=, &position_list] () mutable {
      vector<int> pos_vec = lua_script_->GetVector<int>("position");
      position_list.push_back({pos_vec[0], pos_vec[1]});
    });
    deployer_ = new Deployer(position_list);
  }
}

Game::~Game() {
  if (lua_script_ != nullptr) delete lua_script_;
  if (map_ != nullptr) delete map_;

  for (auto o : units_) delete o;
  for (auto o : dead_units_) delete o;
}

void Game::InitLua(const string& stage_script_path) {
  lua_script_ = new LuaScript();

  // Register game object as lua global
  lua_script_->SetRawPointerToGlobal(LUA_GAME_OBJ_NAME, (void*)this);

#define GAME_PREFIX "$" LUA_GAME_TABLE_NAME "."

  // Register API functions
#define MACRO_LUA_GAME(cname, luaname) \
  lua_script_->Set(GAME_PREFIX #luaname, Game_##cname);

#include "lua_game.inc.h"

#undef MACRO_LUA_GAME

  // Register enum values
  lua_script_->Set(GAME_PREFIX "side.own", (int)Unit::kSideOwn);
  lua_script_->Set(GAME_PREFIX "side.ally", (int)Unit::kSideAlly);
  lua_script_->Set(GAME_PREFIX "side.enemy", (int)Unit::kSideEnemy);
  lua_script_->Set(GAME_PREFIX "end_type.undecided", (int)Status::kUndecided);
  lua_script_->Set(GAME_PREFIX "end_type.defeat", (int)Status::kDefeat);
  lua_script_->Set(GAME_PREFIX "end_type.victory", (int)Status::kVictory);

#undef GAME_PREFIX

  // Run the main script
  lua_script_->Run(stage_script_path);
}

void Game::ForEachUnit(std::function<void(Unit*)> f) {
  for (auto o : units_) {
    if (o != nullptr)
      f(o);
  }
}

void Game::MoveUnit(Unit* unit, Vec2D dst) {
  Vec2D src = unit->GetPosition();
  if (src == dst) return;
  map_->MoveUnit(src, dst);
  unit->SetPosition(dst);
}

void Game::KillUnit(Unit* unit) {
  for (int i = 0, sz = units_.size(); i < sz; i++) {
    if (unit == units_[i]) {
      map_->RemoveUnit(unit->GetPosition());
      units_.erase(units_.begin() + i);
      dead_units_.push_back(unit);
      break;
    }
  }
}

bool Game::TryBasicAttack(Unit* unit_atk, Unit* unit_def) {
  return GenRandom(100) < Formulae::ComputeBasicAttackAccuracy(unit_atk, unit_def);
}

bool Game::TryMagic(Unit* unit_atk, Unit* unit_def) {
  return GenRandom(100) < Formulae::ComputeMagicAccuracy(unit_atk, unit_def);
}

bool Game::IsValidCoords(Vec2D c) {
  return map_->IsValidCoords(c);
}

Magic* Game::GetMagic(const std::string& id) {
  return rc_.magic_manager->Get(id);
}

Unit* Game::GetUnit(int id) {
  return units_[id];
}

Equipment* Game::GetEquipment(const std::string& id) {
  return rc_.equipment_manager->Get(id);
}

bool Game::EndSideTurn() {
  ForEachUnit([this] (Unit* u) {
    if (this->IsCurrentTurn(u)) {
      u->ResetAction();
    }
  });

  bool next_turn = turn_.Next();

  ForEachUnit([this] (Unit* u) {
    if (this->IsCurrentTurn(u)) {
      u->RaiseEvent(EventEffect::Type::kOnTurnBegin);
    }
  });

  if (next_turn) {
    // TODO do stuff when next turn begins (when actually the turn number increased)
  }

  return IsUserTurn();
}

bool Game::IsUserTurn() const {
  return turn_.GetSide() == Unit::kSideOwn;
}

bool Game::IsAITurn() const {
  return !IsUserTurn();
}

bool Game::IsCurrentTurn(Unit* unit) const {
  return unit->GetSide() == turn_.GetSide();
}

uint16_t Game::GetTurnCurrent() const {
  return turn_.GetCurrent();
}

uint16_t Game::GetTurnLimit() const {
  return turn_.GetLimit();
}

vector<Unit*> Game::GetCurrentUnits() {
  vector<Unit*> units;
  ForEachUnit([this, &units] (Unit* u) {
    if (this->IsCurrentTurn(u)) {
      units.push_back(u);
    }
  });
  return units;
}

vector<Vec2D> Game::FindMovablePos(Unit* unit) {
  PathTree* path_tree = FindMovablePath(unit);
  return path_tree->GetNodeList();
}

PathTree* Game::FindMovablePath(Unit* unit) {
  return map_->FindMovablePath(unit);
}

Unit* Game::GetOneHostileInRange(Unit* unit, Vec2D virtual_pos) {
  Vec2D original_pos = unit->GetPosition();
  MoveUnit(unit, virtual_pos);
  Unit* target = nullptr;
  for (auto candidate : units_) {
    if (unit->IsHostile(candidate)) {
      if (unit->IsInRange(candidate->GetPosition())) {
        target = candidate;
        break;
      }
    }
  }
  MoveUnit(unit, original_pos);
  return target;
}

bool Game::HasPendingCmd() const {
  return commander_.HasPendingCmd();
}

const Cmd* Game::GetNextCmdConst() const {
  ASSERT(HasPendingCmd());
  return commander_.GetNextCmdConst();
}

bool Game::UnitInCell(Vec2D c) const {
  return map_->UnitInCell(c);
}

void Game::DoPendingCmd() {
  ASSERT(HasPendingCmd());
#ifdef DEBUG
  commander_.DebugPrint();
#endif
  commander_.DoNextCmd(this);
}

void Game::PushCmd(unique_ptr<Cmd> cmd) {
///  if (cmd == nullptr) return;
  commander_.PushCmd(std::move(cmd));
}

bool Game::CheckStatus() {
  if (status_ != Status::kUndecided) return false;
  uint32_t res = lua_script_->Call<uint32_t>("$end_condition");
  status_ = static_cast<Status>(res);
  return (status_ != Status::kUndecided);
}

uint32_t Game::GetNumEnemiesAlive() {
  uint32_t count = 0;
  ForEachUnit([=, &count] (Unit* u) {
    if (u->GetSide() == Unit::kSideEnemy) {
      count++;
    }
  });
  return count;
}

uint32_t Game::GetNumOwnsAlive() {
  uint32_t count = 0;
  ForEachUnit([=, &count] (Unit* u) {
    if (u->GetSide() == Unit::kSideOwn) {
      count++;
    }
  });
  return count;
}

void Game::Begin() {
  ASSERT(status_ == Status::kDeploying);
  status_ = Status::kUndecided;

  // TODO genreate own units deployed

  lua_script_->Call<void>("$on_begin");
}

void Game::AppointHero(const string& id, uint16_t level) {
  LOG_INFO("Hero added to asset '%s' with Lv %d", id.c_str(), level);
  auto hero = std::make_shared<Hero>(rc_.hero_tpl_manager->Get(id), level);
  assets_->AddHero(hero);
}

uint32_t Game::GenerateOwnUnit(const string& id, Vec2D pos) {
  shared_ptr<Hero> hero = assets_->GetHero(id);
  return GenerateOwnUnit(hero, pos);
}

uint32_t Game::GenerateOwnUnit(shared_ptr<Hero> hero, Vec2D pos) {
  Unit* unit = new Unit(hero, Unit::kSideOwn);
  units_.push_back(unit);
  map_->PlaceUnit(unit, pos);
  return units_.size() - 1;
}

uint32_t Game::GenerateUnit(const string& id, uint16_t level, Unit::Side side, Vec2D pos) {
  HeroTemplate* hero_tpl = rc_.hero_tpl_manager->Get(id);
  auto hero = std::make_shared<Hero>(hero_tpl, level);
  Unit* unit = new Unit(hero, side);
  units_.push_back(unit);
  map_->PlaceUnit(unit, pos);
  return units_.size() - 1;
}

void Game::ObtainEquipment(const string& id, uint32_t amount) {
  Equipment* eq = rc_.equipment_manager->Get(id);
  assets_->AddEquipment(eq, amount);
}

bool Game::UnitPutWeaponOn(uint32_t unit_id, const string& weapon_id) {
  Unit*      unit   = GetUnit(unit_id);
  Equipment* weapon = assets_->GetEquipment(weapon_id);
  if (weapon == nullptr) return false;
  assets_->RemoveEquipment(weapon_id, 1);
  unit->PutWeaponOn(weapon);
  return true;
}

void Game::SubmitDeploy() {
  deployer_->ForEach([=] (const DeployElement& e) {
    this->GenerateOwnUnit(e.hero, deployer_->ConvertToPosition(e.no));
  });
}

uint32_t Game::AssignDeploy(const shared_ptr<Hero>& hero) {
  return deployer_->Assign(hero);
}

void Game::UnassignDeploy(const shared_ptr<Hero>& hero) {
  deployer_->Unassign(hero);
}

