#ifndef MAGIC_LIST_H_
#define MAGIC_LIST_H_

#include "util/common.h"
#include "resource_manager.h"

namespace mengde {
namespace core {

class Magic;
class Unit;

class MagicList {
 public:
  MagicList();
  MagicList(MagicManager*, Unit*);
  void Clear();
  void AddMagicsForUnit(MagicManager*, Unit*);
  int NumMagics();
  Magic* GetMagic(int);

 private:
  vector<Magic*> magics_;
};

} // namespace core
} // namespace mengde

#endif // MAGIC_LIST_H_
