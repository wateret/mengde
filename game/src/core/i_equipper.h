#ifndef MENGDE_CORE_I_EQUIPPER_H_
#define MENGDE_CORE_I_EQUIPPER_H_

namespace mengde {
namespace core {

class UnitClass;
class EquipmentSet;

class IEquipper {
 public:
  virtual ~IEquipper()      = default;
  virtual void UpdateStat() = 0;
  //  void UpdateHpMp() = 0;
  virtual const UnitClass* GetClass() const = 0;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_I_EQUIPPER_H_
