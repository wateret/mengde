#ifndef EVENT_EFFECT_H_
#define EVENT_EFFECT_H_

#include "util/common.h"
#include <limits>

namespace mengde {
namespace core {

class Unit;

// EventEffect class is an observer for game event handling

class EventEffect {
 public:
  static const uint16_t kTurnInfinity = std::numeric_limits<uint16_t>::max();

 public:
  enum class Type {
    kOnNone,
    kOnTurnBegin,
    kOnPreBasicAttack,
    kOnPreBasicAttacked,
    kOnDamaged,
    kOnActionDone
  };

 public:
  EventEffect(Type, uint16_t turn_left = kTurnInfinity);
  virtual ~EventEffect();
  virtual void OnEvent(Unit*) = 0;
  bool type(Type type) { return type_ == type; }
  int GetTurnsLeft() { return turns_left_; }
  void NextTurn();

 private:
  Type type_;
  uint16_t turns_left_;
};

class EERestoreHP : public EventEffect {
 public:
  EERestoreHP(Type, int);
  virtual void OnEvent(Unit*) override;

 private:
  int amount_;
};

} // namespace core
} // namespace mengde

#endif // EVENT_EFFECT_H_
