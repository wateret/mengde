#ifndef EVENT_EFFECT_H_
#define EVENT_EFFECT_H_

class Unit;

class EventEffect {
 public:
  enum EventEffectType {
    kOnNone,
    kOnTurnBegin,
    kOnReceiveDamage,
    kOnActionDone
  };

 public:
  EventEffect(EventEffectType);
  virtual ~EventEffect();
  virtual void OnEvent(Unit*, void*) = 0;
  bool IsOfType(EventEffectType type) { return type_ == type; }
  int GetTurnsLeft() { return turns_left_; }
  void NextTurn() { --turns_left_; }

 private:
  EventEffectType type_;
  int turns_left_;
};

class EERestoreHPAfterAction : public EventEffect {
 public:
   struct EEData {
   };

 public:
  EERestoreHPAfterAction(int);
  virtual void OnEvent(Unit*, void*) override;

 private:
  int amount_;
};

class EERestoreHPOnTurnBegin : public EventEffect {
 public:
   struct EEData {
   };

 public:
  EERestoreHPOnTurnBegin(int);
  virtual void OnEvent(Unit*, void*) override;

 private:
  int amount_;
};

#endif // EVENT_EFFECT_H_
