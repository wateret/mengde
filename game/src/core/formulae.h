#ifndef MENGDE_CORE_FORMULAE_H_
#define MENGDE_CORE_FORMULAE_H_

#include <stdint.h>

#include "attribute_modifier.h"

namespace mengde {
namespace core {
class Unit;
class Map;
}  // namespace core
}  // namespace mengde

namespace mengde {
namespace core {
namespace formulae {

const int kDefaultRatio = 100;

int ComputeBasicAttackDamage(const Map*, const Unit*, const Unit*, int = kDefaultRatio);
int ComputeMagicDamage(const Map*, const Unit*, const Unit*, int = kDefaultRatio);
int ComputeBasicAttackAccuracy(const Unit*, const Unit*, int = kDefaultRatio);
int ComputeMagicAccuracy(const Unit*, const Unit*, int = kDefaultRatio);
int ComputeBasicAttackDouble(const Unit*, const Unit*);
int ComputeBasicAttackCritical(const Unit*, const Unit*);
int ComputeAttributeChange(int value, AttributeChange change);
uint32_t ComputeExp(const Unit*, const Unit*);
int ApplyRatio(int, int);

}  // namespace formulae
}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_FORMULAE_H_
