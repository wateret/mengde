#include "formulae.h"

#include "map.h"
#include "unit.h"
#include "util/common.h"

namespace mengde {
namespace core {
namespace formulae {

static int ComputeDamageBase(int atk, int def, int atk_lv, int force) {
  int damage = (atk - def) / 3 + atk_lv + 25;
  damage = ApplyRatio(damage, force);
  return std::max(1, damage);
}

static int ComputeDoubleCriticalBase(int atk, int def) {
  int val = 0;
  if (atk >= def * 3) {
    val = 100;
  } else if (atk >= def * 2) {
    val = (atk - def * 2) * 80 / def + 20;
  } else if (atk >= def) {
    val = (atk - def) * 18 / def + 2;
  } else {
    val = 1;
  }
  return val;
}

static int ComputeAccuracyBase(int atk, int def, int cap) {
  int val = 0;
  if (atk >= def / 3) {
    val = std::min(100, (atk - def) * 10 / def + 90);
  } else if (atk >= def / 2) {
    int tdef = def / 2;
    val = (atk - tdef) * 30 / tdef + 60;
  } else {
    int tdef = def / 3;
    val = std::max(atk - tdef, 0) * 30 / tdef + 30;
  }
  val = ApplyRatio(val, cap);
  return val;
}

int ComputeBasicAttackDamage(const Map* m, const Unit* unit_atk, const Unit* unit_def, int force) {
  const Attribute& a = unit_atk->GetCurrentAttr();
  const Attribute& d = unit_def->GetCurrentAttr();
  int atk = m->ApplyTerrainEffect(unit_atk, a.atk);
  int def = m->ApplyTerrainEffect(unit_def, d.def);
  return ComputeDamageBase(atk, def, unit_atk->GetLevel(), force);
}

int ComputeMagicDamage(const Map* m, const Unit* unit_atk, const Unit* unit_def, int force) {
  UNUSED(m);
  const Attribute& a = unit_atk->GetCurrentAttr();
  const Attribute& d = unit_def->GetCurrentAttr();
  int atk = a.itl;
  int def = d.itl;
  return ComputeDamageBase(atk, def, unit_atk->GetLevel(), force);
}

int ComputeBasicAttackAccuracy(const Unit* unit_atk, const Unit* unit_def, int cap) {
  const Attribute& a = unit_atk->GetCurrentAttr();
  const Attribute& d = unit_def->GetCurrentAttr();
  return ComputeAccuracyBase(a.dex, d.dex, cap);
}

int ComputeMagicAccuracy(const Unit* unit_atk, const Unit* unit_def, int cap) {
  const Attribute& a = unit_atk->GetCurrentAttr();
  const Attribute& d = unit_def->GetCurrentAttr();
  return ComputeAccuracyBase(a.itl + a.mor, d.itl + d.mor, cap);
}

int ComputeBasicAttackDouble(const Unit* unit_atk, const Unit* unit_def) {
  const Attribute& a = unit_atk->GetCurrentAttr();
  const Attribute& d = unit_def->GetCurrentAttr();
  return ComputeDoubleCriticalBase(a.dex, d.dex);
}

int ComputeBasicAttackCritical(const Unit* unit_atk, const Unit* unit_def) {
  const Attribute& a = unit_atk->GetCurrentAttr();
  const Attribute& d = unit_def->GetCurrentAttr();
  return ComputeDoubleCriticalBase(a.mor, d.mor);
}

int ComputeAttributeChange(int value, AttributeChange change) {
  return ApplyRatio(value, change.multiplier) + change.addend;
}

// ratio is a percentage value, 100 is default
int ApplyRatio(int value, int ratio) { return value * ratio / 100; }

uint32_t ComputeExp(const Unit* doer, const Unit* doee) {
  int level_diff = doee->GetLevel() - doer->GetLevel();
  uint16_t exp = 0;
  if (level_diff < 0) {
    exp = std::max(1, 16 + level_diff);
  } else {
    exp = std::min(200, 16 + 4 * level_diff);
  }
  return exp;
}

}  // namespace formulae
}  // namespace core
}  // namespace mengde
