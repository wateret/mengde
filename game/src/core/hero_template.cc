#include "hero_template.h"

namespace mengde {
namespace core {

HeroTemplate::HeroTemplate(const string& id, const HeroClass* unit_class, const Attribute& hero_stat)
    : id_(id), unit_class_(unit_class), hero_attr_(hero_stat) {}

int HeroTemplate::class_index() const { return unit_class_->index(); }

int HeroTemplate::move() const { return unit_class_->move(); }

const AttackRange& HeroTemplate::attack_range() const { return unit_class_->attack_range(); }

}  // namespace core
}  // namespace mengde
