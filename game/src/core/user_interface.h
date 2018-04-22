#ifndef MENGDE_CORE_USER_INTERFACE_H_
#define MENGDE_CORE_USER_INTERFACE_H_

#include <stdint.h>

namespace mengde {
namespace core {

class Game;
class Scenario;
class Unit;

class UserInterface {
 public:
  UserInterface(Game* stage);

 private:
  Unit* GetUnit(uint32_t unit_id);

 private:
  Game* stage_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_USER_INTERFACE_H_
