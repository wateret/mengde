#include "cmd.h"

#include <algorithm>

#include "ai_executor.h"
#include "cmd_visitor.h"
#include "core/path_tree.h"
#include "formulae.h"
#include "lua_game.h"
#include "magic.h"
#include "stage.h"
#include "user_interface.h"

namespace mengde {
namespace core {

const char* kCmdOpToString[] = {"Invalid",
#define MACRO_CMD_OP(ename) #ename,
#include "cmd_op.h.inc"
                                nullptr};

//
// Accept methods for all nodes
//

#define MACRO_CMD_OP(name) \
  void Cmd##name::Accept(CmdVisitor& visitor) const { visitor.Visit(*this); }
#include "cmd_op.h.inc"

// CmdUnit

CmdUnit::CmdUnit(const UId& unit) : unit_(unit) { ASSERT(unit_); }

// CmdTwoUnits

CmdTwoUnits::CmdTwoUnits(const UId& atk, const UId& def) : atk_(atk), def_(def) {
  ASSERT(atk_);  // def_(second unit) could be none
}

void CmdTwoUnits::SwapAtkDef() { std::swap(atk_, def_); }

// CmdAct

CmdAct::CmdAct(const UId& atk, const UId& def) : CmdTwoUnits(atk, def) {}

// CmdStay

CmdStay::CmdStay(const UId& unit) : CmdAct(unit, UId{}) {}

unique_ptr<Cmd> CmdStay::Do(Stage*) {
  // Do nothing
  return nullptr;
}

// CmdEndAction
CmdEndAction::CmdEndAction(const UId& unit) : CmdUnit(unit) {}

unique_ptr<Cmd> CmdEndAction::Do(Stage* game) {
  auto unit = game->LookupUnit(unit_);
  unit->EndAction();
  game->Push(unit->RaiseEvent(event::GeneralEvent::kActionDone));

  game->RunEvents();

  return nullptr;
}

// CmdBasicAttack

CmdBasicAttack::CmdBasicAttack(const UId& atk, const UId& def, Type type)
    : CmdAct(atk, def), type_(type), multiplier_(0), addend_(0) {
  // Either one of kActive or kCounter must be set
  ASSERT(type & Type::kActiveOrCounter);
  // Cannot set flag kActive and kCounter at the same time
  ASSERT((type & Type::kActiveOrCounter) != Type::kActiveOrCounter);
}

unique_ptr<Cmd> CmdBasicAttack::Do(Stage* game) {
  auto atk = game->LookupUnit(atk_);
  auto def = game->LookupUnit(def_);

  if (atk->IsDead() || def->IsDead()) return nullptr;

  Vec2D atk_pos = atk->position();
  Vec2D def_pos = def->position();
  Direction dir = Vec2DRelativePosition(atk_pos, def_pos);
  atk->direction(dir);
  def->direction(OppositeDirection(dir));

  LOG_INFO("%s(%s) '%s' -> '%s'", IsCounter() ? "CounterAttack" : "Attack", IsSecond() ? "2nd" : "1st",
           atk->id().c_str(), def->id().c_str());

  CmdQueue* ret = new CmdQueue();

  if (!IsCounter()) {
    atk->RaiseEvent(event::OnCmdEvent::kNormalAttack, this);
    def->RaiseEvent(event::OnCmdEvent::kNormalAttacked, this);
  } else {
    atk->RaiseEvent(event::OnCmdEvent::kCounterAttack, this);
    def->RaiseEvent(event::OnCmdEvent::kCounterAttacked, this);
  }

  // Perform and get result
  bool success = TryBasicAttack(game);
  if (success) {
    CmdHit::HitType hit_type = TryBasicAttackCritical(game) ? CmdHit::HitType::kCritical : CmdHit::HitType::kNormal;
    int damage = ComputeDamage(game, game->GetMap());
    if (hit_type == CmdHit::HitType::kCritical) {
      damage = damage * 3 / 2;  // 1.5x
    }
    if (IsSecond()) {
      damage = damage * 3 / 4;  // 0.75x
    }
    damage = std::max(damage, 1);
    ret->Append(std::make_unique<CmdHit>(atk_, def_, CmdActResult::Type::kBasicAttack, hit_type, damage));
  } else {
    ret->Append(std::make_unique<CmdMiss>(atk_, def_, CmdActResult::Type::kBasicAttack));
  }

  // Double attack
  bool reserve_second_attack = TryBasicAttackDouble(game);
  if (!IsSecond() && reserve_second_attack) {
    ret->Append(std::make_unique<CmdBasicAttack>(atk_, def_, (Type)(type_ | Type::kSecond)));
  }

  // Counter attack
  bool is_last_attack = (reserve_second_attack == IsSecond());
  if (is_last_attack && !IsCounter() && def->IsInRange(atk->position()) &&
      !def->condition_set().Has(Condition::kStunned)) {
    LOG_INFO("'%s's' counter-attack to '%s' is reserved.", def->id().c_str(), atk->id().c_str());
    ret->Append(std::make_unique<CmdBasicAttack>(def_, atk_, CmdBasicAttack::Type::kCounter));
  }

  return unique_ptr<Cmd>(ret);
}

bool CmdBasicAttack::TryBasicAttack(Stage* stage) {
  auto atk = stage->LookupUnit(atk_);
  auto def = stage->LookupUnit(def_);

  int chance = Formulae::ComputeBasicAttackAccuracy(atk, def);
  LOG_INFO("Chance of Hit : %d%", chance);
  return GenRandom(100) < chance;
}

bool CmdBasicAttack::TryBasicAttackCritical(Stage* stage) {
  auto atk = stage->LookupUnit(atk_);
  auto def = stage->LookupUnit(def_);

  int chance = Formulae::ComputeBasicAttackCritical(atk, def);
  LOG_INFO("Chance of Critical : %d%", chance);
  return GenRandom(100) < chance;
}

bool CmdBasicAttack::TryBasicAttackDouble(Stage* stage) {
  auto atk = stage->LookupUnit(atk_);
  auto def = stage->LookupUnit(def_);

  int chance = Formulae::ComputeBasicAttackDouble(atk, def);
  LOG_INFO("Chance of Double Attack : %d%", chance);
  return GenRandom(100) < chance;
}

int CmdBasicAttack::ComputeDamage(Stage* stage, Map* map) {
  auto atk = stage->LookupUnit(atk_);
  auto def = stage->LookupUnit(def_);

  int damage = Formulae::ComputeBasicAttackDamage(map, atk, def);
  damage += addend_;
  damage = damage * (100 + multiplier_) / 100;
  damage = std::max(damage, 0);
  return damage;
}

// CmdMagic

CmdMagic::CmdMagic(const UId& atk, const UId& def, Magic* magic) : CmdAct(atk, def), magic_(magic) {}

unique_ptr<Cmd> CmdMagic::Do(Stage* stage) {
  auto atk = stage->LookupUnit(atk_);
  auto def = stage->LookupUnit(def_);

  LOG_INFO("'%s' tries magic '%s' to '%s'", atk->id().c_str(), magic_->GetId().c_str(), def->id().c_str());
  bool hit = magic_->TryPerform(atk, def);
  Cmd* ret = nullptr;
  if (hit) {
    int hp_diff = 0;
    if (magic_->HasHP()) {
      const auto atk = stage->LookupUnit(atk_);
      const auto def = stage->LookupUnit(def_);
      hp_diff = magic_->HPDiff(atk, def);
    }
    ret = new CmdHit(atk_, def_, CmdActResult::Type::kMagic, CmdHit::HitType::kNormal, magic_, hp_diff);
  } else {
    ret = new CmdMiss(atk_, def_, CmdActResult::Type::kMagic, magic_);
  }

  return unique_ptr<Cmd>(ret);
}

// CmdActResult

CmdActResult::CmdActResult(const UId& atk, const UId& def, Type type, Magic* magic)
    : CmdTwoUnits(atk, def), type_(type), magic_(magic) {}

CmdActResult::CmdActResult(const UId& atk, const UId& def, Type type) : CmdActResult(atk, def, type, nullptr) {
  ASSERT(type == Type::kBasicAttack);
}

// CmdHit

CmdHit::CmdHit(const UId& atk, const UId& def, Type type, HitType hit_type, Magic* magic, int damage)
    : CmdActResult(atk, def, type, magic), hit_type_(hit_type), damage_(damage) {}

CmdHit::CmdHit(const UId& atk, const UId& def, Type type, HitType hit_type, int damage)
    : CmdActResult(atk, def, type), hit_type_(hit_type), damage_(damage) {}

unique_ptr<Cmd> CmdHit::Do(Stage* stage) {
  auto atk = stage->LookupUnit(atk_);
  auto def = stage->LookupUnit(def_);

  uint32_t exp_factor{1u};
  CmdQueue* ret = new CmdQueue;

  if (type_ == Type::kBasicAttack) {
    const string hit_type = (hit_type_ == HitType::kCritical) ? "Critical" : "Normal";
    LOG_INFO("%s does damage to %s by %d (%s)", atk->id().c_str(), def->id().c_str(), damage_, hit_type.c_str());

    if (!def->DoDamage(damage_)) {  // unit is dead
      ret->Append(std::make_unique<CmdKilled>(def_));
      exp_factor = 3u;
    }
  } else {
    ASSERT(type_ == Type::kMagic);
    magic_->Perform(atk, def);
  }

  // Gain experience
  {
    uint32_t exp = Formulae::ComputeExp(atk, def) * exp_factor;
    if (exp > 0) {
      ret->Append(std::make_unique<CmdGainExp>(atk_, exp));
    }
  }

  return unique_ptr<Cmd>{ret};
}

// CmdMiss

CmdMiss::CmdMiss(const UId& atk, const UId& def, Type type, Magic* magic) : CmdActResult(atk, def, type, magic) {}

CmdMiss::CmdMiss(const UId& atk, const UId& def, Type type) : CmdActResult(atk, def, type) {}

unique_ptr<Cmd> CmdMiss::Do(Stage* stage) {
  auto atk = stage->LookupUnit(atk_);
  auto def = stage->LookupUnit(def_);

  LOG_INFO("%s misses!", stage->LookupUnit(atk_)->id().c_str());

  // Gain experience
  {
    uint32_t exp = Formulae::ComputeExp(atk, def) / 2;
    if (exp > 0) {
      return std::make_unique<CmdGainExp>(atk_, exp);
    }
  }

  return nullptr;
}

// CmdKilled

CmdKilled::CmdKilled(const UId& unit) : CmdUnit(unit) {}

unique_ptr<Cmd> CmdKilled::Do(Stage* stage) {
  stage->KillUnit(stage->LookupUnit(unit_));
  return nullptr;
}

// CmdMove

CmdMove::CmdMove(const UId& unit, Vec2D dest) : CmdUnit(unit), dest_(dest) {}

unique_ptr<Cmd> CmdMove::Do(Stage* game) {
  auto unit = game->LookupUnit(unit_);
  LOG_INFO("Unit '%s' moved from (%d, %d) to (%d, %d)", unit->id().c_str(), unit->position().x, unit->position().y,
           dest_.x, dest_.y);
  game->MoveUnit(unit, dest_);
  return nullptr;
}

// CmdAction

CmdAction::CmdAction() : CmdAction(Flag::kDecompose) {}

CmdAction::CmdAction(Flag flag) : cmd_move_(nullptr), cmd_act_(nullptr), flag_(flag) {}

void CmdAction::SetCmdMove(unique_ptr<CmdMove> cmd) { cmd_move_ = std::move(cmd); }

void CmdAction::SetCmdAct(unique_ptr<CmdAct> cmd) { cmd_act_ = std::move(cmd); }
unique_ptr<Cmd> CmdAction::Do(Stage* game) {
  auto doer_id = cmd_act_ ? cmd_act_->GetUnitAtk() : cmd_move_->GetUnit();
  auto doer = game->LookupUnit(doer_id);
  ASSERT(doer != nullptr);

  auto ret = std::make_unique<CmdQueue>();

  if (flag_ == Flag::kNone) {
    UNREACHABLE("Unsupported");
#if 0
    if (cmd_move_ != nullptr) {
      unique_ptr<Cmd> tmp = cmd_move_->Do(game);
      ASSERT(tmp == nullptr);
    }

    if (cmd_act_ != nullptr) {
      unique_ptr<Cmd> tmp = cmd_act_->Do(game);
      ret->Append(std::move(tmp));
    }

    doer->EndAction();
#endif

  } else if (flag_ == Flag::kUserInput) {
    // Do cmd_move_ immediately (Not to play move animation)
    if (cmd_move_ != nullptr) {
      unique_ptr<Cmd> tmp = cmd_move_->Do(game);
      ASSERT(tmp == nullptr);
    }
    if (cmd_act_ != nullptr) {
      ret->Append(std::move(cmd_act_));
    }
  } else {
    ASSERT(flag_ == Flag::kDecompose);
    LOG_DEBUG("DECOMPOSING Action");
    if (cmd_move_ != nullptr) {
      ret->Append(std::move(cmd_move_));
    }
    if (cmd_act_ != nullptr) {
      ret->Append(std::move(cmd_act_));
    }
  }

  ret->Append(unique_ptr<Cmd>(new CmdEndAction(doer_id)));

  // TODO raise move event
  //      Even if it didn't move, we can just send message that we are at some position...
  //      BTW act event must have been notified by `cmd_act_`

  //  ret->Append(commands that were generated - raised by move event);

  return std::move(ret);
}

// CmdEndTurn

CmdEndTurn::CmdEndTurn() : Cmd() {}

unique_ptr<Cmd> CmdEndTurn::Do(Stage* game) {
  game->EndForceTurn();
  return nullptr;
}

// CmdPlayAI

CmdPlayAI::CmdPlayAI() : Cmd() {}

unique_ptr<Cmd> CmdPlayAI::Do(Stage* game) {
  UserInterface* ui = game->user_interface();
  AIExecutor ai_executor{ui};

  return ai_executor.Step();
}

// CmdGameVictory

CmdGameVictory::CmdGameVictory() : Cmd() {}

unique_ptr<Cmd> CmdGameVictory::Do(Stage* game) {
  auto& on_victory = game->lua_callbacks()->on_victory();
  on_victory(*game->lua_game());

  // Push a new CmdScenarioEnd just in case when user script does not specifiy the next scenario
  game->Push(unique_ptr<Cmd>(new CmdGameEnd(true)));

  return nullptr;
}

// CmdGameEnd

CmdGameEnd::CmdGameEnd(bool is_victory) : Cmd(), is_victory_(is_victory) {}

unique_ptr<Cmd> CmdGameEnd::Do(Stage*) {
  // TODO Update Own units info
  return nullptr;
}

// CmdSpeak

CmdSpeak::CmdSpeak(const UId& unit, const string& words) : CmdUnit(unit), words_(words) {}

unique_ptr<Cmd> CmdSpeak::Do(Stage*) {
  // Do nothing, UI will do appropriate stuff.
  return nullptr;
}

// CmdRestoreHp

CmdRestoreHp::CmdRestoreHp(const UId& unit, int ratio, int adder) : CmdUnit(unit), ratio_(ratio), adder_(adder) {}

unique_ptr<Cmd> CmdRestoreHp::Do(Stage* stage) {
  auto unit = stage->LookupUnit(unit_);
  int amount = CalcAmount(stage->user_interface());
  unit->RestoreHP(amount);
  LOG_INFO("%s restores HP by %d", unit->id().c_str(), amount);
  return nullptr;
}

int CmdRestoreHp::CalcAmount(UserInterface* gi) const {
  auto unit = gi->GetUnit(unit_);

  int amount = unit->GetOriginalHpMp().hp * ratio_ / 100;
  amount += adder_;
  return std::min(amount, unit->GetOriginalHpMp().hp - unit->GetCurrentHpMp().hp);
}

// CmdGainExp

CmdGainExp::CmdGainExp(const UId& unit, uint32_t exp) : CmdUnit(unit), exp_(exp) {}

unique_ptr<Cmd> CmdGainExp::Do(Stage* game) {
  auto unit = game->LookupUnit(unit_);
  unique_ptr<Cmd> ret;
  if (unit->GainExp(exp_)) {
    ret = std::make_unique<CmdLevelUp>(unit_);
  }
  LOG_INFO("%s gains exp by %u", unit->id().c_str(), exp_);
  return ret;
}

// CmdLevelUp

CmdLevelUp::CmdLevelUp(const UId& unit) : CmdUnit(unit) {}

unique_ptr<Cmd> CmdLevelUp::Do(Stage* game) {
  auto unit = game->LookupUnit(unit_);
  unique_ptr<Cmd> ret;
  unit->LevelUp();
  if (unit->ReadyPromotion()) {
    ret = std::make_unique<CmdPromote>(unit_);
  }
  return ret;
}

// CmdLevelUp

CmdPromote::CmdPromote(const UId& unit) : CmdUnit(unit) {}

unique_ptr<Cmd> CmdPromote::Do(Stage* game) {
  auto unit = game->LookupUnit(unit_);
  ASSERT(unit->ReadyPromotion());
  unit->Promote(game->unit_class_manager());
  return nullptr;
}

}  // namespace core
}  // namespace mengde
