#ifndef STAT_MODIFIER_H_
#define STAT_MODIFIER_H_

#include <string>

namespace mengde {
namespace core {

class StatModifier {
 public:
  StatModifier(const std::string&, uint16_t, uint16_t, uint16_t, uint16_t = UINT16_MAX);
  std::string GetId() const { return id_; }
  uint16_t GetStatId() const { return stat_id_; }
  int16_t GetAddend() const { return addend_; }
  uint16_t GetMultiplier() const { return multiplier_; }
  uint16_t GetTurnsLeft() const { return turns_left_; }
  void NextTurn();

 private:
  std::string id_;
  uint16_t stat_id_;
  int16_t addend_;
  uint16_t multiplier_;
  uint16_t turns_left_;
};

} // namespace core
} // namespace mengde

#endif // STAT_MODIFIER_H_
