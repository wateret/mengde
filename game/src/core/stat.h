#ifndef STAT_H_
#define STAT_H_

#include <stdint.h>

namespace mengde {
namespace core {

struct Attribute {
  enum {
    kStatIdAtk,
    kStatIdDef,
    kStatIdDex,
    kStatIdItl,
    kStatIdMor
  };

  int atk;
  int def;
  int dex;
  int itl;
  int mor;

  Attribute()
      : atk(0), def(0), dex(0), itl(0), mor(0) {
  }

  Attribute(int atk, int def, int dex, int itl, int mor)
      : atk(atk), def(def), dex(dex), itl(itl), mor(mor) {
  }

  int GetValueByIndex(int index) const {
    // Fragile but fast implementation
    return *((int*)(this) + index);
  }

  void AddValueByIndex(int index, int value) {
    // Fragile but fast implementation
    ((int*)(this))[index] += value;
  }

  void SetValueByIndex(int index, int value) {
    // Fragile but fast implementation
    ((int*)(this))[index] = value;
  }
};

Attribute operator+(const Attribute& lhs, const Attribute& rhs);
Attribute operator+(const Attribute& lhs, int rhs);
Attribute& operator+=(Attribute& lhs, const Attribute& rhs);
Attribute& operator*=(Attribute& lhs, const Attribute& rhs);
Attribute& operator/=(Attribute& lhs, int rhs);

#define NUM_STATS (sizeof(Attribute) / sizeof(int))

struct HpMp {
  int hp;
  int mp;

  HpMp() : hp(0), mp(0) {
  }

  HpMp(int hp, int mp) : hp(hp), mp(mp) {
  }

  int GetValueByIndex(int index) const {
    // Fragile but fast implementation
    return *((int*)(this) + index);
  }
};

struct Level {
  static const uint16_t kExpLimit = 100;

  uint16_t level;
  uint16_t exp;

  Level(uint16_t level, uint16_t exp) : level(level), exp(exp) {}
};

} // namespace core
} // namespace mengde

#endif // STAT_H_
