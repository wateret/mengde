#include "formulae.h"

#include "util/common.h"
#include "unit.h"
#include "map.h"

namespace mengde {
namespace core {

Formulae::Formulae() {
}

int Formulae::ComputeBasicAttackDamage(Map* m, Unit* unit_atk, Unit* unit_def, int force) {
  const Attribute& a = unit_atk->GetCurrentStat();
  const Attribute& d = unit_def->GetCurrentStat();
  int atk = m->ApplyTerrainEffect(unit_atk, a.atk);
  int def = m->ApplyTerrainEffect(unit_def, d.def);
  return ComputeDamageBase(atk, def, unit_atk->GetLevel(), force);
}

int Formulae::ComputeMagicDamage(Map* m, Unit* unit_atk, Unit* unit_def, int force) {
  UNUSED(m);
  const Attribute& a = unit_atk->GetCurrentStat();
  const Attribute& d = unit_def->GetCurrentStat();
  int atk = a.itl;
  int def = d.itl;
  return ComputeDamageBase(atk, def, unit_atk->GetLevel(), force);
}

int Formulae::ComputeBasicAttackAccuracy(Unit* unit_atk, Unit* unit_def, int cap) {
  const Attribute& a = unit_atk->GetCurrentStat();
  const Attribute& d = unit_def->GetCurrentStat();
  return ComputeAccuracyBase(a.dex, d.dex, cap);
}

int Formulae::ComputeMagicAccuracy(Unit* unit_atk, Unit* unit_def, int cap) {
  const Attribute& a = unit_atk->GetCurrentStat();
  const Attribute& d = unit_def->GetCurrentStat();
  return ComputeAccuracyBase(a.itl + a.mor, d.itl + d.mor, cap);
}

int Formulae::ComputeBasicAttackDouble(Unit* unit_atk, Unit* unit_def) {
  const Attribute& a = unit_atk->GetCurrentStat();
  const Attribute& d = unit_def->GetCurrentStat();
  return ComputeDoubleCriticalBase(a.dex, d.dex);
}

int Formulae::ComputeBasicAttackCritical(Unit* unit_atk, Unit* unit_def) {
  const Attribute& a = unit_atk->GetCurrentStat();
  const Attribute& d = unit_def->GetCurrentStat();
  return ComputeDoubleCriticalBase(a.mor, d.mor);
}

int Formulae::ComputeDamageBase(int atk, int def, int atk_lv, int force) {
  int damage = (atk - def) / 3 + atk_lv + 25;
  damage = ApplyRatio(damage, force);
  return std::max(1, damage);
}

int Formulae::ComputeDoubleCriticalBase(int atk, int def) {
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

int Formulae::ComputeAccuracyBase(int atk, int def, int cap) {
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

// ratio is a percentage value, 100 is default
int Formulae::ApplyRatio(int value, int ratio) {
  return value * ratio / 100;
}

} // namespace core
} // namespace mengde
