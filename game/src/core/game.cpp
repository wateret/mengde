#include "game.h"
#include "game_env.h"
#include "magic.h"
#include "config_loader.h"
#include "formulae.h"
#include "cmd.h"
#include "util/path_tree.h"
#include "lua/lua_script.h"

// XXX temporary include
#include "event_effect.h"
#include "lua_game.h"

Game::Game(ConfigLoader* config_loader)
    : rc_(config_loader->GetResources()),
      lua_script_(nullptr),
      commander_(),
      map_(nullptr),
      units_(),
      dead_units_(),
      turn_() {
  InitLua();

  // Read map data
  vector<uint32_t> rc = lua_script_->GetVector<uint32_t>("$gdata.map.size");
  uint32_t rows = rc[0];
  uint32_t cols = rc[1];
  vector<string> map_input = lua_script_->GetVector<string>("$gdata.map.terrain");
  ASSERT(rows == map_input.size());
  for (auto e : map_input)
    ASSERT(cols == e.size());
  map_ = new Map(map_input, "map.bmp", rc_.terrain_manager);

  lua_script_->Call<void>("$initialize");
}

Game::~Game() {
  if (lua_script_ != nullptr) delete lua_script_;
  if (map_ != nullptr) delete map_;

  delete rc_.unit_class_manager;
  delete rc_.terrain_manager;
  delete rc_.magic_manager;
  delete rc_.item_manager;
  delete rc_.hero_manager;

  for (auto o : units_) delete o;
  for (auto o : dead_units_) delete o;
}

void Game::InitLua() {
  lua_script_ = new LuaScript();

  // Register game object as lua global
  lua_script_->SetRawPointerToGlobal(LUA_GAME_OBJ_NAME, (void*)this);

  // Register API functions
#define MACRO_LUA_GAME(cname, luaname) \
  lua_script_->Set("$C." #luaname, Game_##cname);

#include "lua_game.inc.h"

#undef MACRO_LUA_GAME

  // Register enum values
  lua_script_->Set("$C.enums.side.own", (int)Unit::kSideOwn);
  lua_script_->Set("$C.enums.side.ally", (int)Unit::kSideAlly);
  lua_script_->Set("$C.enums.side.enemy", (int)Unit::kSideEnemy);
  lua_script_->Set("$C.enums.end_type.undecided", (int)EndType::kUndecided);
  lua_script_->Set("$C.enums.end_type.lose", (int)EndType::kLose);
  lua_script_->Set("$C.enums.end_type.win", (int)EndType::kWin);

  // Run the main script
  string lua_script_path = GameEnv::GetInstance()->GetScenarioPath() + "/01.lua";
  lua_script_->Run(lua_script_path);
}

void Game::ForEachUnit(std::function<void(Unit*)> f) {
  for (auto o : units_) {
    if (o != nullptr)
      f(o);
  }
}

void Game::MoveUnit(Unit* unit, Vec2D dst) {
  Vec2D src = unit->GetCoords();
  if (src == dst) return;
  map_->MoveUnit(src, dst);
  unit->SetCoords(dst);
}

void Game::KillUnit(Unit* unit) {
  for (int i = 0, sz = units_.size(); i < sz; i++) {
    if (unit == units_[i]) {
      map_->RemoveUnit(unit->GetCoords());
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

Item* Game::GetItem(const std::string& id) {
  return rc_.item_manager->Get(id);
}

bool Game::EndSideTurn() {
  LOG_DEBUG("END SIDE TURN");
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

  switch (CheckEndCondition()) {
    case EndType::kUndecided:
      LOG_DEBUG("Undecided!");
      break;
    case EndType::kLose:
      LOG_DEBUG("Lose!");
      UNREACHABLE("NYI");
      break;
    case EndType::kWin:
      LOG_DEBUG("Wins!");
      PushCmd(unique_ptr<CmdGameWin>(new CmdGameWin()));
      break;
    default:
      break;
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
  Vec2D original_pos = unit->GetCoords();
  MoveUnit(unit, virtual_pos);
  Unit* target = nullptr;
  for (auto candidate : units_) {
    if (unit->IsHostile(candidate)) {
      if (unit->IsInRange(candidate->GetCoords())) {
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

Game::EndType Game::CheckEndCondition() {
  uint32_t res = lua_script_->Call<uint32_t>("$end_condition");
  return (Game::EndType)res;
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

int Game::GenerateUnit(const string& hero_id, Unit::Side side, Vec2D pos) {
  Hero* hero = rc_.hero_manager->Get(hero_id);
  Unit* unit = new Unit(hero, side);
  units_.push_back(unit);
  map_->PlaceUnit(unit, pos);
  return (int)units_.size() - 1;
}

