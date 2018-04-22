#include "user_interface.h"

#include "game.h"
#include "unit.h"

namespace mengde {
namespace core {

UserInterface::UserInterface(Game* stage) : stage_(stage) {}

Unit* UserInterface::GetUnit(uint32_t unit_id) { return stage_->GetUnit(unit_id); }

}  // namespace core
}  // namespace mengde
