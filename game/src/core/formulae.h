#ifndef FORMULAE_H_
#define FORMULAE_H_

class Unit;
class Map;

class Formulae {
 public:
  static const int kDefaultRatio = 100;

 public:
  static int ComputeBasicAttackDamage(Map*, Unit*, Unit*, int = kDefaultRatio);
  static int ComputeMagicDamage(Map*, Unit*, Unit*, int = kDefaultRatio);
  static int ComputeBasicAttackAccuracy(Unit*, Unit*, int = kDefaultRatio);
  static int ComputeMagicAccuracy(Unit*, Unit*, int = kDefaultRatio);
  static int ComputeBasicAttackDouble(Unit*, Unit*);
  static int ComputeBasicAttackCritical(Unit*, Unit*);
  static int ApplyRatio(int, int);

 private:
  static int ComputeDamageBase(int, int, int, int);
  static int ComputeAccuracyBase(int, int, int);
  static int ComputeDoubleCriticalBase(int, int);
  Formulae(); // Prevent instantiation
};

#endif // FORMULAE_H_
