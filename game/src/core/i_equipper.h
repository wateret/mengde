#ifndef I_EQUIPPER_H_
#define I_EQUIPPER_H_

namespace mengde {
namespace core {

class UnitClass;

class IEquipper {
 public:
  virtual void UpdateStat() = 0;
//  void UpdateXtat() = 0;
  virtual const UnitClass* GetClass() const = 0;
};

} // namespace core
} // namespace mengde

#endif // I_EQUIPPER_H_
