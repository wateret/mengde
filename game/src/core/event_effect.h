#ifndef EVENT_EFFECT_H_
#define EVENT_EFFECT_H_

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
  virtual void OnEvent(Unit*, void*) = 0;
  bool IsOfType(Type type) { return type_ == type; }
  int GetTurnsLeft() { return turns_left_; }
  void NextTurn() { --turns_left_; }

 private:
  Type type_;
  int turns_left_;
};

class EERestoreHP : public EventEffect {
 public:
   struct EEData {
   };

 public:
  EERestoreHP(Type, int);
  virtual void OnEvent(Unit*, void*) override;

 private:
  int amount_;
};

#endif // EVENT_EFFECT_H_
