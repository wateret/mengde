#ifndef MENGDE_CORE_UNIT_H_
#define MENGDE_CORE_UNIT_H_

#include <boost/optional.hpp>

#include "equipment_set.h"
#include "event_effect_list.h"
#include "force.h"
#include "hero.h"
#include "i_equipper.h"
#include "i_event.h"
#include "i_unit_base.h"
#include "id.h"
#include "stat_modifier_list.h"
#include "util/common.h"

namespace mengde {
namespace core {

class UnitClass;
class EquipmentSet;

class Unit : public IUnitBase, public IEvent, public IEquipper {
 public:
  Unit(const Hero*, Force);
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
  virtual const UnitClass* unit_class() const override;
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
  void uid(const UId& uid) { uid_ = uid; }
  UId uid() const { return uid_; }
  uint16_t max_exp() { return Level::kExpLimit; }
  void position(Vec2D pos) { position_ = pos; }
  Vec2D position() const { return position_; }
  void direction(Direction direction) { direction_ = direction; }
  Direction direction() const { return direction_; }
  int class_index() const;
  Force force() const { return force_; }

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
  void EndAction();
  void ResetAction();

 private:
  UId uid_;
  Hero* hero_;
  EquipmentSet* equipment_set_;
  Attribute current_attr_;
  HpMp current_hpmp_;
  StatModifierList modifier_list_;
  EventEffectList effect_list_;

  // TODO Consider below variables to move (to gui or another core module)
  Vec2D position_;       // Position on the map
  Direction direction_;  // Direction the unit is looking at
  Force force_;          // Which force that the unit is belong to
  bool done_action_;     // action is done in current turn
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_UNIT_H_
