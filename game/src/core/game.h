#ifndef GAME_H_
#define GAME_H_

#include <string>
#include <vector>
#include <functional>

#include "i_deploy_helper.h"
#include "map.h"
#include "unit.h"
#include "util/common.h"
#include "turn.h"
#include "resource_manager.h"

namespace lua {
  class LuaScript;
}

namespace mengde {
namespace core {

class Assets;
class Cmd;
class Commander;
class LuaScript;
class Magic;
class Deployer;
class StageUnitManager;
class UnitSupervisor;

class Game : public IDeployHelper {
 public:
  enum class Status {
    kNone,
    kDeploying,
    kUndecided,
    kVictory,
    kDefeat
  };

 public:
  Game(const ResourceManagers&, Assets*, const string&);
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
  Unit* GetUnit(uint32_t);
  Equipment* GetEquipment(const std::string&);
  MagicManager* GetMagicManager() { return rc_.magic_manager; }
  lua::LuaScript* GetLuaScript() { return lua_script_; }
  bool EndForceTurn();
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

  // IDeployHelper interfaces
  bool     SubmitDeploy() override;
  uint32_t AssignDeploy(const shared_ptr<Hero>&) override;
  uint32_t UnassignDeploy(const shared_ptr<Hero>&) override;
  uint32_t FindDeploy(const shared_ptr<Hero>& hero) override;

  // APIs for Lua //
  void AppointHero(const string&, uint16_t);
  uint32_t GenerateOwnUnit(const string&, Vec2D);
  uint32_t GenerateOwnUnit(shared_ptr<Hero>, Vec2D);
  uint32_t GenerateUnit(const string&, uint16_t, Force, Vec2D);
  void ObtainEquipment(const string&, uint32_t);
//  bool UnitPutWeaponOn(uint32_t, const string&);

  // APIs for AI //
  vector<Unit*> GetCurrentUnits();
  vector<Vec2D> FindMovablePos(Unit*);
  PathTree*     FindMovablePath(Unit*);
  Unit*         GetOneHostileInRange(Unit*, Vec2D);

 private:
  lua::LuaScript* CreateLua(const string&);
  Map*       CreateMap();
  Deployer*  CreateDeployer();
  bool TryBasicAttack(Unit*, Unit*);
  bool TryMagic(Unit*, Unit*);

 private:
  ResourceManagers   rc_;
  Assets*            assets_;
  lua::LuaScript*         lua_script_;
  Commander*         commander_;
  Deployer*          deployer_;
  Map*               map_;
  StageUnitManager*  stage_unit_manager_;
  Turn               turn_;
  Status             status_;
};

} // namespace core
} // namespace mengde

#endif
