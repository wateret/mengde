#ifndef STAT_H_
#define STAT_H_

struct Stat {
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

  Stat()
      : atk(0), def(0), dex(0), itl(0), mor(0) {
  }

  Stat(int atk, int def, int dex, int itl, int mor)
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

  Stat operator+(const Stat& o) {
    return {atk + o.atk,
            def + o.def,
            dex + o.dex,
            itl + o.itl,
            mor + o.mor};
  }
};

#define NUM_STATS (sizeof(Stat) / sizeof(int))

struct Xtat {
  int hp;
  int mp;
  int exp;

  Xtat() : hp(0), mp(0), exp(0) {
  }

  Xtat(int hp, int mp, int exp) : hp(hp), mp(mp), exp(exp) {
  }

  int GetValueByIndex(int index) const {
    // Fragile but fast implementation
    return *((int*)(this) + index);
  }
};

#endif // STAT_H_
