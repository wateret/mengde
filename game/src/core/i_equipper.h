#ifndef I_EQUIPPER_H_
#define I_EQUIPPER_H_

class UnitClass;

class IEquipper {
 public:
  virtual void UpdateStat() = 0;
//  void UpdateXtat() = 0;
  virtual const UnitClass* GetClass() const = 0;
};

#endif // I_EQUIPPER_H_
