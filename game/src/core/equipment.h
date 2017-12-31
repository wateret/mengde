#ifndef EQUIPMENT_H_
#define EQUIPMENT_H_

#include <string>
#include "unit_effect_list.h"
#include "util/common.h"
#include "i_event.h"

class EventEffect;
class StatModifier;

class Equipment : public IEvent {
 public:
  enum class Type {
    kNone,
    kWeapon,
    kArmor,
    kAid
  };

 public:
  void RaiseEvent(EventEffect::Type, Unit*) override;

 public:
  Equipment(const std::string&, Type);
  string GetId() const { return id_; }
  Type GetType() const { return type_; }
  void AddModifier(StatModifier*);
  void AddEffect(EventEffect*);
  Stat CalcModifierAddends() const;
  Stat CalcModifierMultipliers() const;

 private:
  string id_;
  Type type_;
  UnitEffectList unit_effect_list_;
};

#endif // EQUIPMENT_H_
