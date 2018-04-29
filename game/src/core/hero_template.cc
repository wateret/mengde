#include "hero_template.h"

namespace mengde {
namespace core {

HeroTemplate::HeroTemplate(const string& id, const string& bitmap_path, const UnitClass* unit_class,
                           const Attribute& hero_stat)
    : id_(id), bitmap_path_(bitmap_path), unit_class_(unit_class), hero_attr_(hero_stat) {}

int HeroTemplate::GetClassIndex() const { return unit_class_->GetIndex(); }

int HeroTemplate::GetMove() const { return unit_class_->GetMove(); }

const AttackRange& HeroTemplate::GetAttackRange() const { return unit_class_->GetAttackRange(); }

}  // namespace core
}  // namespace mengde
