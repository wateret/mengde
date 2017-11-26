#ifndef STAT_MODIFIER_H_
#define STAT_MODIFIER_H_

#include <string>

class StatModifier {
 public:
  StatModifier(const std::string&, uint16_t, uint16_t, uint16_t, uint16_t = UINT16_MAX);
  std::string GetId() { return id_; }
  int16_t GetStatId() { return stat_id_; }
  int16_t GetAddend() { return addend_; }
  int16_t GetMultiplier() { return multiplier_; }
  int16_t GetTurnsLeft() { return turns_left_; }
  void NextTurn();

 private:
  std::string id_;
  uint16_t stat_id_;
  uint16_t addend_;
  uint16_t multiplier_;
  uint16_t turns_left_;
};

#endif // STAT_MODIFIER_H_
