#ifndef MENGDE_CORE_USER_INTERFACE_H_
#define MENGDE_CORE_USER_INTERFACE_H_

#include <stdint.h>

#include <boost/optional.hpp>
#include <utility>

#include "cmd.h"
#include "util/common.h"

namespace mengde {
namespace core {

class Cell;
class Game;
class PathTree;
class Scenario;
class Unit;

class AvailableUnits {
 public:
  AvailableUnits(Game* stage);
  UId Get(uint32_t idx);
  uint32_t Count() const { return units_.size(); }
  boost::optional<uint32_t> FindByPos(Vec2D pos);

 private:
  vector<std::pair<uint32_t, Vec2D>> units_;
};

class AvailableMoves {
 public:
  AvailableMoves(Game* stage, uint32_t unit_id);
  Vec2D Get(uint32_t idx);
  uint32_t Count() const { return moves_.size(); }
  void ForEach(const std::function<void(uint32_t, Vec2D)>& fn);
  const vector<Vec2D>& moves() { return moves_; }

 private:
  vector<Vec2D> moves_;
};

class AvailableActs {
 public:
  AvailableActs(Game* stage, uint32_t unit_id, uint32_t move_id, ActionType type);
  ActionType type() { return type_; }
  unique_ptr<CmdAct> Get(uint32_t idx);
  uint32_t Count() const { return acts_.size(); }
  boost::optional<uint32_t> Find(Vec2D pos);
  boost::optional<uint32_t> FindMagic(const string& magic_id, Vec2D pos);

 private:
  Game* stage_;  // TODO Remove this
  ActionType type_;
  vector<unique_ptr<CmdAct>> acts_;
};

class UserInterface {
 public:
  UserInterface(Game* stage);

 public:
  AvailableUnits QueryUnits() const;
  AvailableMoves QueryMoves(uint32_t unit_key);
  AvailableActs QueryActs(uint32_t unit_key, uint32_t move_id, ActionType type);
  void PushAction(uint32_t unit_key, uint32_t move_id, ActionType type, uint32_t act_id);

  const Unit* GetUnit(const UId& uid) const;
  const Unit* GetUnit(uint32_t unit_key) const;
  const Unit* GetUnit(Vec2D pos) const;
  const Cell* GetCell(Vec2D pos) const;
  vector<Vec2D> GetPath(const UId& unit_id, Vec2D pos) const;

  Vec2D GetMapSize() const;
  string GetMapId() const;

  bool HasNextCmd() const;

  void ForEachUnit(const std::function<void(uint32_t, const Unit*)>& fn) const;

 private:
  Vec2D GetMovedPosition(uint32_t unit_id, uint32_t move_id);
  unique_ptr<CmdAct> GetActCmd(uint32_t unit_id, uint32_t move_id, ActionType type, uint32_t act_id);

 private:
  Game* stage_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_USER_INTERFACE_H_
