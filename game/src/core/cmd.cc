#include "cmd.h"

#include <algorithm>

#include "cmd_visitor.h"
#include "core/path_tree.h"
#include "formulae.h"
#include "lua/lua.h"
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

// CmdQueue

unique_ptr<Cmd> CmdQueue::Do(Stage* game) {
  ASSERT(!IsEmpty());

  /*
  // Processing nested CmdQueue is disabled
  Cmd* current = q_.front().get();
  if (CmdQueue* cq = dynamic_cast<CmdQueue*>(current)) {
    // Remove entry if CmdQueue is empty
    if (cq->IsEmpty()) {
      q_.pop_front();
      return Do(game);
    }
  }
  */

  unique_ptr<Cmd> current = std::move(q_.front());
  q_.pop_front();

  // Nested CmdQueue is not allowed
  ASSERT(dynamic_cast<CmdQueue*>(current.get()) == nullptr);

  unique_ptr<Cmd> result = current->Do(game);
  if (result != nullptr) {
    Prepend(std::move(result));
  }

  if (game->CheckStatus()) {
    switch (game->GetStatus()) {
      case Stage::Status::kUndecided:
        LOG_DEBUG("Undecided!");
        break;
      case Stage::Status::kDefeat:
        LOG_DEBUG("Defeat!");
        break;
      case Stage::Status::kVictory:
        LOG_DEBUG("Victory!");
        game->Push(std::make_unique<CmdGameVictory>());
        break;
      default:
        break;
    }
  }

  return current;
}

void CmdQueue::Insert(unique_ptr<Cmd> cmd, bool prepend) {
  if (cmd == nullptr) return;
  if (CmdQueue* cmdq = dynamic_cast<CmdQueue*>(cmd.get())) {
    if (prepend) {
      // In order to keep the sequence of CmdQueue being appended, push to front in reverse order
      for (auto itr = cmdq->q_.rbegin(); itr != cmdq->q_.rend(); itr++) {
        q_.push_front(std::move(*itr));
      }
    } else {
      for (auto&& e : cmdq->q_) {
        q_.push_back(std::move(e));
      }
    }
  } else {
    if (prepend)
      q_.push_front(std::move(cmd));
    else
      q_.push_back(std::move(cmd));
  }
}

void CmdQueue::Append(unique_ptr<Cmd> cmd) { return Insert(std::move(cmd), false); }

void CmdQueue::Prepend(unique_ptr<Cmd> cmd) { return Insert(std::move(cmd), true); }

CmdQueue& CmdQueue::operator+=(unique_ptr<Cmd> rhs) {
  Append(std::move(rhs));
  return *this;
}

bool CmdQueue::IsEmpty() const {
  return q_.empty();
  /*
  // Processing nested CmdQueue is disabled
  for (auto&& e : q) {
    if (CmdQueue* cmdq = dynamic_cast<CmdQueue*>(e.get())) {
      if (!cmdq->IsEmpty()) return false;
    } else {
      return false;
    }
  }
  */
}

const Cmd* CmdQueue::GetNextCmdConst() const {
  ASSERT(!IsEmpty());

  return q_.front().get();

  /*
  // Processing nested CmdQueue is disabled
  Cmd* next = q_.front().get();
  if (CmdQueue* cq = dynamic_cast<CmdQueue*>(next)) {
    return cq->GetNextAtomicCmd();
  } else {
    return next;
  }
  */
}

#ifdef DEBUG
void CmdQueue::DebugPrint(Stage* stage) const {
  printf("CmdQueue {\n");
  for (auto&& e : q_) {
    e->DebugPrint(stage);
  }
  printf("}\n");
}
#endif

// CmdUnit

CmdUnit::CmdUnit(const UId& unit) : unit_(unit) { ASSERT(unit_); }

// CmdTwoUnits

CmdTwoUnits::CmdTwoUnits(const UId& atk, const UId& def) : atk_(atk), def_(def) {
  ASSERT(atk_);  // def_(second unit) could be none
}

#ifdef DEBUG
void CmdTwoUnits::DebugPrint(Stage* stage) const {
  string atk = (!atk_) ? "N/A" : stage->GetUnit(atk_)->id();
  string def = (!def_) ? "N/A" : stage->GetUnit(def_)->id();
  printf("%s (atk:%s def:%s)\n", kCmdOpToString[static_cast<int>(op())], atk.c_str(), def.c_str());
}
#endif

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
  auto unit = game->GetUnit(unit_);
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
  auto atk = game->GetUnit(atk_);
  auto def = game->GetUnit(def_);

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
  if (is_last_attack && !IsCounter() && def->IsInRange(atk->position())) {
    LOG_INFO("'%s's' counter-attack to '%s' is reserved.", def->id().c_str(), atk->id().c_str());
    ret->Append(std::make_unique<CmdBasicAttack>(def_, atk_, CmdBasicAttack::Type::kCounter));
  }

  return unique_ptr<Cmd>(ret);
}

bool CmdBasicAttack::TryBasicAttack(Stage* stage) {
  auto atk = stage->GetUnit(atk_);
  auto def = stage->GetUnit(def_);

  int chance = Formulae::ComputeBasicAttackAccuracy(atk, def);
  LOG_INFO("Chance of Hit : %d%", chance);
  return GenRandom(100) < chance;
}

bool CmdBasicAttack::TryBasicAttackCritical(Stage* stage) {
  auto atk = stage->GetUnit(atk_);
  auto def = stage->GetUnit(def_);

  int chance = Formulae::ComputeBasicAttackCritical(atk, def);
  LOG_INFO("Chance of Critical : %d%", chance);
  return GenRandom(100) < chance;
}

bool CmdBasicAttack::TryBasicAttackDouble(Stage* stage) {
  auto atk = stage->GetUnit(atk_);
  auto def = stage->GetUnit(def_);

  int chance = Formulae::ComputeBasicAttackDouble(atk, def);
  LOG_INFO("Chance of Double Attack : %d%", chance);
  return GenRandom(100) < chance;
}

int CmdBasicAttack::ComputeDamage(Stage* stage, Map* map) {
  auto atk = stage->GetUnit(atk_);
  auto def = stage->GetUnit(def_);

  int damage = Formulae::ComputeBasicAttackDamage(map, atk, def);
  damage += addend_;
  damage = damage * (100 + multiplier_) / 100;
  damage = std::max(damage, 0);
  return damage;
}

// CmdMagic

CmdMagic::CmdMagic(const UId& atk, const UId& def, Magic* magic) : CmdAct(atk, def), magic_(magic) {}

unique_ptr<Cmd> CmdMagic::Do(Stage* stage) {
  auto atk = stage->GetUnit(atk_);
  auto def = stage->GetUnit(def_);

  LOG_INFO("'%s' tries magic '%s' to '%s'", atk->id().c_str(), magic_->GetId().c_str(), def->id().c_str());
  bool hit = magic_->TryPerform(atk, def);
  Cmd* ret = nullptr;
  if (hit) {
    int damage = ComputeDamage(stage->GetMap(), atk, def);
    ret = new CmdHit(atk_, def_, CmdActResult::Type::kMagic, CmdHit::HitType::kNormal, magic_, damage);
  } else {
    ret = new CmdMiss(atk_, def_, CmdActResult::Type::kMagic, magic_);
  }

  return unique_ptr<Cmd>(ret);
}

int CmdMagic::ComputeDamage(Map* map, const Unit* atk, const Unit* def) {
  int damage = Formulae::ComputeMagicDamage(map, atk, def);
  return damage;
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
  auto atk = stage->GetUnit(atk_);
  auto def = stage->GetUnit(def_);

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
  auto atk = stage->GetUnit(atk_);
  auto def = stage->GetUnit(def_);

  LOG_INFO("%s misses!", stage->GetUnit(atk_)->id().c_str());

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
  stage->KillUnit(stage->GetUnit(unit_));
  return nullptr;
}

// CmdMove

CmdMove::CmdMove(const UId& unit, Vec2D dest) : CmdUnit(unit), dest_(dest) {}

unique_ptr<Cmd> CmdMove::Do(Stage* game) {
  auto unit = game->GetUnit(unit_);
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
  auto doer = game->GetUnit(doer_id);
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
  AvailableUnits units = ui->QueryUnits();

  // XXX create an AI handler module and delegate to it.
  // Currently a simple rushing AI is implemented here.

  // No more unit waiting for command
  if (units.Count() == 0) {
    return unique_ptr<Cmd>(new CmdEndTurn());
  }

  const UnitKey ukey{0u};

  AvailableMoves moves = ui->QueryMoves(ukey);

  bool found_target = false;
  MoveKey mkey = 0;
  moves.ForEach([&](const MoveKey& mk, Vec2D) {
    AvailableActs acts = ui->QueryActs(ukey, mk, ActionType::kBasicAttack);
    if (acts.Count() > 0 && !found_target) {
      found_target = true;
      mkey = mk;
    }
  });

  if (found_target) {
    ui->PushAction(ukey, mkey, ActionType::kBasicAttack, 0 /* Simply choose first one */);
  } else {
    ui->PushAction(ukey, GenRandom(moves.Count()), ActionType::kStay, 0);
  }

  return nullptr;
}

// CmdGameVictory

CmdGameVictory::CmdGameVictory() : Cmd() {}

unique_ptr<Cmd> CmdGameVictory::Do(Stage* game) {
  lua::Lua* lua = game->GetLuaScript();
  lua->Call<void>(game->lua_callbacks()->on_victory(), game->lua_this());

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
  auto unit = stage->GetUnit(unit_);
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
  auto unit = game->GetUnit(unit_);
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
  auto unit = game->GetUnit(unit_);
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
  auto unit = game->GetUnit(unit_);
  ASSERT(unit->ReadyPromotion());
  unit->Promote(game->GetUnitClassManager());
  return nullptr;
}

}  // namespace core
}  // namespace mengde
