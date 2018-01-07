#ifndef STAT_H_
#define STAT_H_

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

  Attribute operator+(const Attribute& o) {
    return {atk + o.atk,
            def + o.def,
            dex + o.dex,
            itl + o.itl,
            mor + o.mor};
  }
};

#define NUM_STATS (sizeof(Attribute) / sizeof(int))

struct HpMp {
  int hp;
  int mp;
  int exp;

  HpMp() : hp(0), mp(0), exp(0) {
  }

  HpMp(int hp, int mp, int exp) : hp(hp), mp(mp), exp(exp) {
  }

  int GetValueByIndex(int index) const {
    // Fragile but fast implementation
    return *((int*)(this) + index);
  }
};

} // namespace core
} // namespace mengde

#endif // STAT_H_
