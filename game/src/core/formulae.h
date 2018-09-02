#ifndef MENGDE_CORE_FORMULAE_H_
#define MENGDE_CORE_FORMULAE_H_

#include <stdint.h>

namespace mengde {
namespace core {

class Unit;
class Map;

class Formulae {
 public:
  static const int kDefaultRatio = 100;

 public:
  static int ComputeBasicAttackDamage(const Map*, const Unit*, const Unit*, int = kDefaultRatio);
  static int ComputeMagicDamage(const Map*, const Unit*, const Unit*, int = kDefaultRatio);
  static int ComputeBasicAttackAccuracy(const Unit*, const Unit*, int = kDefaultRatio);
  static int ComputeMagicAccuracy(const Unit*, const Unit*, int = kDefaultRatio);
  static int ComputeBasicAttackDouble(const Unit*, const Unit*);
  static int ComputeBasicAttackCritical(const Unit*, const Unit*);
  static uint32_t ComputeExp(const Unit*, const Unit*);
  static int ApplyRatio(int, int);

 private:
  static int ComputeDamageBase(int, int, int, int);
  static int ComputeAccuracyBase(int, int, int);
  static int ComputeDoubleCriticalBase(int, int);
  Formulae();  // Prevent instantiation
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_FORMULAE_H_
