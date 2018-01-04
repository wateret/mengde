#ifndef EVENT_EFFECT_H_
#define EVENT_EFFECT_H_

namespace mengde {
namespace core {

class Unit;

// EventEffect class is an observer for game event handling

class EventEffect {
 public:
  enum class Type {
    kOnNone,
    kOnTurnBegin,
    kOnReceiveDamage,
    kOnActionDone
  };

 public:
  EventEffect(Type);
  virtual ~EventEffect();
  virtual void OnEvent(Unit*) = 0;
  bool IsOfType(Type type) { return type_ == type; }
  int GetTurnsLeft() { return turns_left_; }
  void NextTurn() { --turns_left_; }

 private:
  Type type_;
  int turns_left_;
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
