#ifndef GAME_H_
#define GAME_H_

#include "commander.h"
#include "map.h"
#include "unit.h"
#include "common.h"
#include "turn.h"
#include "resource_manager.h"
#include <string>
#include <vector>
#include <functional>

class LuaScript;
class Magic;
class ConfigLoader;

class Game {
 public:
  enum class Status {
    kNone,
    kUndecided,
    kVictory,
    kDefeat
  };

 public:
  Game(ConfigLoader*);
  ~Game();

 public:
  // Map related //
  Map* GetMap() { return map_; }
  Vec2D GetMapSize() { return map_->GetSize(); }
  std::string GetMapBitmapPath() { return map_->GetBitmapPath(); }

  // General //
  void ForEachUnit(std::function<void(Unit*)>);
  void MoveUnit(Unit*, Vec2D);
  void KillUnit(Unit*);
  Cmd* BasicAttackUnit(Unit*, Unit*, bool);
  bool IsValidCoords(Vec2D);
  Magic* GetMagic(const std::string&);
  Unit* GetUnit(int);
  Item* GetItem(const std::string&);
  MagicManager* GetMagicManager() { return rc_.magic_manager; }
  LuaScript* GetLuaScript() { return lua_script_; }
  bool EndSideTurn();
  bool IsCurrentTurn(Unit*) const;
  bool IsAITurn() const;
  bool IsUserTurn() const;
  uint16_t GetTurnCurrent() const;
  uint16_t GetTurnLimit() const;
  bool HasPendingCmd() const;
  void DoPendingCmd();
  void PushCmd(unique_ptr<Cmd>);
  const Cmd* GetNextCmdConst() const;
  bool UnitInCell(Vec2D) const;
  uint32_t GetNumEnemiesAlive();
  uint32_t GetNumOwnsAlive();
  bool CheckStatus();
  Status GetStatus() { return status_; }

  int GenerateUnit(const string&, Unit::Side, Vec2D);

  // AI related API //
  vector<Unit*> GetCurrentUnits();
  vector<Vec2D> FindMovablePos(Unit*);
  PathTree*     FindMovablePath(Unit*);
  Unit*         GetOneHostileInRange(Unit*, Vec2D);

 private:
  void InitLua();
  bool TryBasicAttack(Unit*, Unit*);
  bool TryMagic(Unit*, Unit*);

 private:
  ResourceManagers   rc_;
  LuaScript*         lua_script_;
  Commander          commander_;
  Map*               map_;
  std::vector<Unit*> units_;
  std::vector<Unit*> dead_units_;
  Turn               turn_;
  Status             status_;
};

#endif
