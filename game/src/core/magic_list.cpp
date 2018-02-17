#include "magic_list.h"

#include "magic.h"
#include "unit.h"

namespace mengde {
namespace core {

MagicList::MagicList() {}

MagicList::MagicList(MagicManager* mm, Unit* unit) { AddMagicsForUnit(mm, unit); }

void MagicList::Clear() { magics_.clear(); }

void MagicList::AddMagicsForUnit(MagicManager* mm, Unit* unit) {
  mm->ForEach([this, unit](Magic* magic) {
    if (magic->IsAvailible(unit)) {
      this->magics_.push_back(magic);
    }
  });
}

int MagicList::NumMagics() { return magics_.size(); }

Magic* MagicList::GetMagic(int index) {
  ASSERT(index >= 0 && index < NumMagics());
  return magics_[index];
}

}  // namespace core
}  // namespace mengde
