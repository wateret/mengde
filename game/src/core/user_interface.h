#ifndef MENGDE_CORE_USER_INTERFACE_H_
#define MENGDE_CORE_USER_INTERFACE_H_

#include <stdint.h>

#include <utility>

#include "cmd.h"
#include "id.h"
#include "util/common.h"

namespace mengde {
namespace core {

class Cell;
class Stage;
class PathTree;
class Scenario;
class Unit;
class Turn;
class MagicList;

class AvailableUnits {
 public:
  AvailableUnits(Stage* stage);
  UId Get(const UnitKey& ukey);
  uint32_t Count() const { return units_.size(); }
  UnitKey FindByPos(Vec2D pos);

 private:
  vector<std::pair<UId, Vec2D>> units_;
};

class AvailableMoves {
 public:
  AvailableMoves(Stage* stage, const UnitKey& ukey);
  Vec2D Get(const MoveKey& mkey);
  uint32_t Count() const { return moves_.size(); }
  void ForEach(const std::function<void(const MoveKey&, Vec2D)>& fn);
  const vector<Vec2D>& moves() { return moves_; }

 private:
  vector<Vec2D> moves_;
};

class AvailableActs {
 public:
  AvailableActs(Stage* stage, const UnitKey& unit_id, const MoveKey& move_id, ActionType type);
  ActionType type() { return type_; }
  unique_ptr<CmdAct> Get(const ActKey& akey);
  uint32_t Count() const { return acts_.size(); }
  ActKey Find(Vec2D pos);
  ActKey FindMagic(const string& magic_id, Vec2D pos);

 private:
  Stage* stage_;  // TODO Remove this
  ActionType type_;
  vector<unique_ptr<CmdAct>> acts_;
};

class UserInterface {
 public:
  UserInterface(Stage* stage);

 public:
  AvailableUnits QueryUnits() const;
  AvailableMoves QueryMoves(const UnitKey& unit_key) const;
  AvailableActs QueryActs(const UnitKey& unit_key, const MoveKey& move_id, ActionType type) const;
  void PushAction(const UnitKey& unit_key, const MoveKey& move_id, ActionType type, const ActKey& act_id);
  void PushPlayAI();

  const Unit* GetUnit(const UId& uid) const;
  const Unit* GetUnit(const UnitKey& unit_key) const;
  const Unit* GetUnit(Vec2D pos) const;
  const Cell* GetCell(Vec2D pos) const;
  vector<Vec2D> GetPath(const UId& unit_id, Vec2D pos) const;

  std::shared_ptr<core::MagicList> GetMagicList(const UId& uid) const;
  const Magic* GetMagic(const string& id) const;
  const HeroClass* GetUnitClass(const string& id) const;

  Vec2D GetMapSize() const;
  string GetMapId() const;
  const Map* GetMap() const;

  bool HasNextCmd() const;
  const Cmd* GetNextCmd() const;
  void DoNextCmd();

  void ForEachUnit(const std::function<void(const Unit*)>& fn) const;

  bool IsUserTurn() const;
  bool IsAITurn() const;
  const Turn& GetTurn() const;

  bool IsValidCoords(Vec2D c) const;

 private:
  Vec2D GetMovedPosition(const UnitKey& unit_id, const MoveKey& move_id);
  unique_ptr<CmdAct> GetActCmd(const UnitKey& unit_id, const MoveKey& move_id, ActionType type, const ActKey& act_id);

 private:
  Stage* stage_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_USER_INTERFACE_H_
