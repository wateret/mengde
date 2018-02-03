#ifndef EVENT_EFFECT_H_
#define EVENT_EFFECT_H_

#include <limits>

#include "util/common.h"
#include "event_type.h"

namespace mengde {
namespace core {

class Unit;

// EventEffect class is an observer for game event handling

class EventEffect {
 public:
  static const uint16_t kTurnInfinity = std::numeric_limits<uint16_t>::max();

 public:
  EventEffect(EventType, uint16_t turn_left = kTurnInfinity);
  virtual ~EventEffect();
  virtual void OnEvent(Unit*) = 0;
  bool type(EventType type) { return type_ == type; }
  int GetTurnsLeft() { return turns_left_; }
  void NextTurn();

 private:
  EventType type_;
  uint16_t turns_left_;
};

class EERestoreHP : public EventEffect {
 public:
  EERestoreHP(EventType, int);
  virtual void OnEvent(Unit*) override;

 private:
  int amount_;
};

} // namespace core
} // namespace mengde

#endif // EVENT_EFFECT_H_
