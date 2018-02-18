#ifndef CORE_I_UNIT_BASE_H_
#define CORE_I_UNIT_BASE_H_

#include <string>

using std::string;

struct Vec2D;

namespace mengde {
namespace core {

struct Attribute;
struct HpMp;
class EquipmentSet;
class UnitClass;

class IUnitBase {
 public:
  virtual ~IUnitBase() = default;

  virtual string              GetId() const           = 0;
  virtual string              GetModelId() const      = 0;
  virtual const UnitClass*    GetClass() const        = 0;
  virtual int                 GetMove() const         = 0;
  virtual Vec2D*              GetAttackRange() const  = 0;
  virtual uint16_t            GetLevel() const        = 0;
  virtual uint16_t            GetExp() const          = 0;
  virtual const HpMp&         GetOriginalHpMp() const = 0;
  virtual const Attribute&    GetOriginalAttr() const = 0;
  virtual const HpMp&         GetCurrentHpMp() const  = 0;
  virtual const Attribute&    GetCurrentAttr() const  = 0;
  virtual const EquipmentSet* GetEquipmentSet() const = 0;
  virtual void                UpdateStat()            = 0;
};

}  // namespace core
}  // namespace mengde

#endif  // CORE_I_UNIT_BASE_H_
