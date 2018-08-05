#ifndef MENGDE_CORE_GAME_H_
#define MENGDE_CORE_GAME_H_

#include <functional>
#include <string>
#include <vector>

#include "i_deploy_helper.h"
#include "lua/lua.h"
#include "lua_callbacks.h"
#include "map.h"
#include "resource_manager.h"
#include "turn.h"
#include "unit.h"
#include "util/common.h"

class Path;

namespace lua {
class Lua;
class Ref;
}  // namespace lua

namespace mengde {
namespace core {

class Assets;
class Cmd;
class Commander;
class LuaCallbacks;
class Magic;
class Deployer;
class StageUnitManager;
class UnitSupervisor;
class UserInterface;

class Stage : public IDeployHelper {
 public:
  enum class Status { kNone, kDeploying, kUndecided, kVictory, kDefeat };

 public:
  Stage(const ResourceManagers&, Assets*, const Path&);
  ~Stage();

 public:
  // Map related //
  Map* GetMap() { return map_; }
  Vec2D GetMapSize() { return map_->GetSize(); }
  std::string GetMapId() { return map_->GetModelId(); }

  // General //
  void ForEachUnitConst(std::function<void(const Unit*)> fn) const;
  void ForEachUnit(std::function<void(Unit*)>);
  void MoveUnit(Unit*, Vec2D);
  void MoveUnit(const UId& uid, Vec2D dst);
  void KillUnit(Unit*);
  bool IsValidCoords(Vec2D);
  Magic* GetMagic(const std::string&);
  Unit* GetUnit(const UId&);
  const Unit* GetUnit(const UId&) const;
  Equipment* GetEquipment(const std::string&);
  MagicManager* GetMagicManager() { return rc_.magic_manager; }
  lua::Lua* GetLuaScript() { return lua_; }
  Force GetCurrentForce() const;
  bool EndForceTurn();
  bool IsCurrentTurn(Unit*) const;
  bool IsAITurn() const;
  bool IsUserTurn() const;
  uint16_t GetTurnCurrent() const;
  uint16_t GetTurnLimit() const;
  bool HasNext() const;
  void DoNext();
  void Push(unique_ptr<Cmd>);
  const Cmd* GetNextCmdConst() const;
  bool UnitInCell(Vec2D) const;
  const Unit* GetUnitInCell(Vec2D) const;
  const Cell* GetCell(Vec2D) const;
  uint32_t GetNumEnemiesAlive();
  uint32_t GetNumOwnsAlive();
  bool CheckStatus();
  Status GetStatus() { return status_; }
  Assets* assets() { return assets_; }

  // IDeployHelper interfaces
  bool SubmitDeploy() override;
  uint32_t AssignDeploy(const Hero*) override;
  uint32_t UnassignDeploy(const Hero*) override;
  uint32_t FindDeploy(const Hero*) override;

  // APIs for Lua //
  void AppointHero(const string&, uint16_t);
  uint32_t GenerateOwnUnit(const string&, Vec2D);
  uint32_t GenerateOwnUnit(const Hero*, Vec2D);
  uint32_t GenerateUnit(const string&, uint16_t, Force, Vec2D);
  void ObtainEquipment(const string&, uint32_t);
  //  bool UnitPutWeaponOn(uint32_t, const string&);
  void SetOnDeploy(const lua::Ref& ref);
  void SetOnBegin(const lua::Ref& ref);
  void SetOnVictory(const lua::Ref& ref);
  void SetOnDefeat(const lua::Ref& ref);
  void SetEndCondition(const lua::Ref& ref);

  // APIs for AI //
  vector<Vec2D> FindMovablePos(Unit*);
  PathTree* FindMovablePath(Unit*);

 public:
  const lua::LuaClass& lua_this() { return lua_this_; }
  UserInterface* user_interface() { return user_interface_.get(); }
  const LuaCallbacks* lua_callbacks() { return lua_callbacks_.get(); }

 private:
  lua::Lua* CreateLua(const Path&);
  Map* CreateMap();
  Deployer* CreateDeployer();
  bool TryBasicAttack(Unit*, Unit*);
  bool TryMagic(Unit*, Unit*);

  void RunCallbacks();

 private:
  ResourceManagers rc_;
  Assets* assets_;
  lua::LuaClass lua_this_;  // LuaClass of this object
  lua::Lua* lua_;
  std::unique_ptr<LuaCallbacks> lua_callbacks_;
  std::unique_ptr<UserInterface> user_interface_;
  std::unique_ptr<Commander> commander_;
  Deployer* deployer_;
  Map* map_;
  std::unique_ptr<StageUnitManager> stage_unit_manager_;
  Turn turn_;
  Status status_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_GAME_H_
