#ifndef UNIT_H_
#define UNIT_H_

#include <string>
#include <vector>
#include "common.h"
#include "hero.h"
#include "stat_modifier_list.h"
#include "item_slot.h"
#include "i_event.h"

class UnitClass;

class Unit : public IEvent {
 public:
  enum Side {
    kSideNone     = 0x00,
    kSideFirst    = 0x01,
    kSideOwn      = 0x01,
    kSideAlly     = 0x02,
    kSideEnemy    = 0x04,
    kSideFriendly = kSideOwn | kSideAlly,
    kSideLast     = 0x08                   // Not a real value
  };

 public:
  Unit(Hero*, const Side);
  ~Unit();

 public:
  void RaiseEvent(EventEffect::Type, Unit*) override;
  void RaiseEvent(EventEffect::Type);

 public:
  std::string GetId();
  int GetLevel();
  int GetMove();
  std::string GetBitmapPath();
  const Stat* GetOriginalStat() const;
  const Xtat* GetOriginalXtat() const;
  const Stat* GetCurrentStat() const { return &current_stat_; }
  const Xtat* GetCurrentXtat() const { return &current_xtat_; }
  void RecalcStat();
  void AddStatModifier(StatModifier*);
  int GetMaxExp() { return 100 + GetLevel() * 5; }
  void SetCoords(Vec2D pos) { position_ = pos; }
  Vec2D GetCoords() { return position_; }
  void SetDirection(Direction direction) { direction_ = direction; }
  Direction GetDirection() { return direction_; }
  void SetNoRender(bool b) { no_render_ = b; }
  const UnitClass* GetClass() const;
  int GetClassIndex() const;
  Side GetSide() const { return side_; }
  Vec2D* GetAttackRange() const;
  bool IsNoRender() const { return no_render_; }
  bool IsHPLow() const;
  bool IsHPZero() const;
  bool DoDamage(int);
  void RestoreHP(int);
  void Heal(int);
  bool IsHostile(Unit*) const;
  bool IsInRange(Vec2D, Vec2D*) const;
  bool IsInRange(Vec2D) const;
  bool IsDoneAction() const { return done_action_; }
  void GainExp(Unit*);
  void GainExp(int exp);
  void LevelUp();
  void EndAction();
  void ResetAction();
  void PutWeaponOn(Item*);
  void PutArmorOn(Item*);
  void PutAidOn(Item*);
  Item* GetWeapon();
  Item* GetArmor();
  Item* GetAid();

 private:
  Hero*     hero_;
  ItemSlot  item_slot_weapon_;
  ItemSlot  item_slot_armor_;
  ItemSlot  item_slot_aid_;
  Stat      current_stat_;
  Xtat      current_xtat_;
  Vec2D     position_;
  Direction direction_;
  StatModifierList stat_modifier_list_;
  EventEffectList  effect_list_;
  Side      side_;
  bool      no_render_;     // FIXME this should be moved to some View related module
  bool      done_action_;
};

#endif // UNIT_H_
