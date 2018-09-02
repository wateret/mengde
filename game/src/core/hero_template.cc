#include "hero_template.h"

namespace mengde {
namespace core {

HeroTemplate::HeroTemplate(const string& id, const UnitClass* unit_class, const Attribute& hero_stat)
    : id_(id), unit_class_(unit_class), hero_attr_(hero_stat) {}

int HeroTemplate::class_index() const { return unit_class_->GetIndex(); }

int HeroTemplate::GetMove() const { return unit_class_->GetMove(); }

const AttackRange& HeroTemplate::GetAttackRange() const { return unit_class_->GetAttackRange(); }

}  // namespace core
}  // namespace mengde
