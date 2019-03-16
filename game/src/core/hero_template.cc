#include "hero_template.h"

namespace mengde {
namespace core {

HeroTemplate::HeroTemplate(const string& id, const HeroClass* hero_class, const Attribute& hero_stat)
    : id_(id), hero_class_(hero_class), hero_attr_(hero_stat) {}

int HeroTemplate::class_index() const { return hero_class_->index(); }

int HeroTemplate::move() const { return hero_class_->move(); }

const AttackRange& HeroTemplate::attack_range() const { return hero_class_->attack_range(); }

}  // namespace core
}  // namespace mengde
