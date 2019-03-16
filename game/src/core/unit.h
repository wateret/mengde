#ifndef MENGDE_CORE_UNIT_H_
#define MENGDE_CORE_UNIT_H_

#include <boost/optional.hpp>

#include "condition_set.h"
#include "equipment_set.h"
#include "force.h"
#include "hero.h"
#include "i_equipper.h"
#include "i_event.h"
#include "i_unit_base.h"
#include "id.h"
#include "resource_manager.h"
#include "util/common.h"
#include "volatile_attribute.h"

namespace mengde {
namespace core {

class HeroClass;
class EquipmentSet;

class Unit : public IUnitBase, public IEvent, public IEquipper {
 public:
  Unit(Hero*, Force);
  virtual ~Unit();

 public:
  // IEvent interfaces and wrappers
  virtual unique_ptr<Cmd> RaiseEvent(event::GeneralEvent, Unit*) const override;
  virtual void RaiseEvent(event::OnCmdEvent, Unit*, CmdAct*) const override;
  unique_ptr<Cmd> RaiseEvent(event::GeneralEvent);
  void RaiseEvent(event::OnCmdEvent, CmdAct*);

 public:
  // IUnitBase interfaces
  virtual string id() const override;
  virtual const HeroClass* hero_class() const override;
  virtual int move() const override;
  virtual const AttackRange& attack_range() const override;
  virtual uint16_t GetLevel() const override;
  virtual uint16_t GetExp() const override;
  virtual const HpMp& GetOriginalHpMp() const override;
  virtual const Attribute& GetOriginalAttr() const override;
  virtual const HpMp& GetCurrentHpMp() const override { return current_hpmp_; }
  virtual const Attribute& GetCurrentAttr() const override { return current_attr_; }
  virtual const EquipmentSet* GetEquipmentSet() const override { return equipment_set_; }
  virtual void UpdateStat() override;

 public:
  const Hero& hero() { return *hero_; }
  void uid(const UId& uid) { uid_ = uid; }
  UId uid() const { return uid_; }
  uint16_t max_exp() { return Level::kExpLimit; }
  void position(Vec2D pos) { position_ = pos; }
  Vec2D position() const { return position_; }
  void direction(Direction direction) { direction_ = direction; }
  Direction direction() const { return direction_; }
  int class_index() const;
  Force force() const { return force_; }
  const VolatileAttribute& volatile_attribute() const { return volatile_attribute_; }
  const ConditionSet& condition_set() const { return condition_set_; }
  ConditionSet& condition_set() { return condition_set_; }

 public:
  void AddStatModifier(StatModifier*);
  void AddEventEffect(EventEffect*);
  bool IsHPLow() const;
  bool IsDead() const;
  bool DoDamage(int);
  void RestoreHP(int);
  void Heal(int);
  void Kill();
  bool IsHostile(const Unit*) const;
  bool IsInRange(Vec2D, const AttackRange&) const;
  bool IsInRange(Vec2D) const;
  bool IsDoneAction() const { return done_action_; }
  bool GainExp(uint16_t exp);
  void LevelUp();
  bool ReadyPromotion() const;
  void Promote(const HeroClassManager* hcm);
  void EndAction();
  void ResetAction();
  void NextTurn();

 private:
  UId uid_;
  Hero* const hero_;
  EquipmentSet* equipment_set_;
  Attribute current_attr_;
  HpMp current_hpmp_;
  VolatileAttribute volatile_attribute_;
  ConditionSet condition_set_;

  // TODO Consider below variables to move (to gui or another core module)
  Vec2D position_;       // Position on the map
  Direction direction_;  // Direction the unit is looking at
  Force force_;          // Which force that the unit is belong to
  bool done_action_;     // action is done in current turn
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_UNIT_H_
