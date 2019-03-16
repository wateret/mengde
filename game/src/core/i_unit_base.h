#ifndef MENGDE_CORE_I_UNIT_BASE_H_
#define MENGDE_CORE_I_UNIT_BASE_H_

#include <string>

using std::string;

struct Vec2D;

namespace mengde {
namespace core {

struct Attribute;
struct HpMp;
class AttackRange;
class EquipmentSet;
class HeroClass;

class IUnitBase {
 public:
  virtual ~IUnitBase() = default;

  virtual string id() const = 0;
  virtual const HeroClass* hero_class() const = 0;
  virtual int move() const = 0;
  virtual const AttackRange& attack_range() const = 0;
  virtual uint16_t GetLevel() const = 0;
  virtual uint16_t GetExp() const = 0;
  virtual const HpMp& GetOriginalHpMp() const = 0;
  virtual const Attribute& GetOriginalAttr() const = 0;
  virtual const HpMp& GetCurrentHpMp() const = 0;
  virtual const Attribute& GetCurrentAttr() const = 0;
  virtual const EquipmentSet* GetEquipmentSet() const = 0;
  virtual void UpdateStat() = 0;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_I_UNIT_BASE_H_
