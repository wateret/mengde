#include "cmd.h"

#include "formulae.h"
#include "game.h"
#include "magic.h"
#include "core/path_tree.h"
#include "lua/olua.h"

namespace mengde {
namespace core {

const char* kCmdOpToString[] = {
#define MACRO_CMD_OP(ename) #ename,
#include "cmd_op.inc.h"
  nullptr
};

// Cmd

Cmd::Cmd() {
}

Cmd::~Cmd() {
}

// CmdQueue

CmdQueue::CmdQueue() : Cmd() {
}

CmdQueue::~CmdQueue() {
}

unique_ptr<Cmd> CmdQueue::Do(Game* game) {
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
    Append(std::move(result));
  }

  if (game->CheckStatus()) {
    switch (game->GetStatus()) {
      case Game::Status::kUndecided:
        LOG_DEBUG("Undecided!");
        break;
      case Game::Status::kDefeat:
        LOG_DEBUG("Defeat!");
        break;
      case Game::Status::kVictory:
        LOG_DEBUG("Victory!");
        game->Push(unique_ptr<CmdGameVictory>(new CmdGameVictory()));
        break;
      default:
        break;
    }
  }

  return current;
}

void CmdQueue::Append(unique_ptr<Cmd> cmd) {
  if (cmd == nullptr) return;
  if (CmdQueue* cmdq = dynamic_cast<CmdQueue*>(cmd.get())) {
    for (auto&& e : cmdq->q_) {
      q_.push_back(std::move(e));
    }
  } else {
    q_.push_back(std::move(cmd));
  }
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
void CmdQueue::DebugPrint() const {
  printf("CmdQueue {\n");
  for (auto&& e : q_) {
    e->DebugPrint();
  }
  printf("}\n");
}
#endif

// CmdOneUnit

CmdOneUnit::CmdOneUnit(Unit* unit) : unit_(unit) {
  ASSERT(unit_ != nullptr);
}

// CmdTwoUnits

CmdTwoUnits::CmdTwoUnits(Unit* atk, Unit* def) : atk_(atk), def_(def) {
  ASSERT(atk_ != nullptr); // def_(second unit) could be nullptr
}

#ifdef DEBUG
void CmdTwoUnits::DebugPrint() const {
  string atk = (atk_ == nullptr) ? "N/A" : atk_->GetId();
  string def = (def_ == nullptr) ? "N/A" : def_->GetId();
  printf("%s (atk:%s def:%s)\n",
         kCmdOpToString[static_cast<int>(GetOp())],
         atk.c_str(),
         def.c_str());
}
#endif

// CmdAct

CmdAct::CmdAct(Unit* atk, Unit* def) : CmdTwoUnits(atk, def) {
}

// CmdStay

CmdStay::CmdStay(Unit* unit) : CmdAct(unit, nullptr) {
}

unique_ptr<Cmd> CmdStay::Do(Game*) {
  atk_->EndAction(); // XXX Temporarily added
  // Simply do nothing
  return nullptr;
}

// CmdBasicAttack

CmdBasicAttack::CmdBasicAttack(Unit* atk, Unit* def, Type type)
    : CmdAct(atk, def), type_(type) {
  // Either one of kActive or kCounter must be set
  ASSERT(type & Type::kActiveOrCounter);
  // Cannot set flag kActive and kCounter at the same time
  ASSERT((type & Type::kActiveOrCounter) != Type::kActiveOrCounter);
}

unique_ptr<Cmd> CmdBasicAttack::Do(Game* game) {
  if (atk_->IsDead() || def_->IsDead()) return nullptr;

  Vec2D atk_pos = atk_->GetPosition();
  Vec2D def_pos = def_->GetPosition();
  Direction dir = Vec2DRelativePosition(atk_pos, def_pos);
  atk_->SetDirection(dir);
  def_->SetDirection(OppositeDirection(dir));

  LOG_INFO("%s(%s) '%s' -> '%s'",
           IsCounter() ? "CounterAttack" : "BasicAttack",
           IsSecond() ? "2nd" : "1st",
           atk_->GetId().c_str(),
           def_->GetId().c_str());

  unique_ptr<CmdQueue> ret(new CmdQueue());
  bool success = TryBasicAttack();
  if (success) {
    CmdHit::HitType hit_type = TryBasicAttackCritical() ? CmdHit::HitType::kCritical : CmdHit::HitType::kNormal;
    int damage = ComputeDamage(game->GetMap());
    if (hit_type == CmdHit::HitType::kCritical) {
      damage *= 1.5;
    }
    if (IsSecond()) {
      damage *= 0.75;
    }
    ret->Append(unique_ptr<CmdHit>(new CmdHit(atk_, def_, CmdActResult::Type::kBasicAttack, hit_type, damage)));
  } else {
    ret->Append(unique_ptr<CmdMiss>(new CmdMiss(atk_, def_, CmdActResult::Type::kBasicAttack)));
  }

  bool reserve_second_attack = TryBasicAttackDouble();
  if (!IsSecond() && reserve_second_attack) {
    ret->Append(unique_ptr<CmdBasicAttack>(new CmdBasicAttack(atk_, def_, (Type)(type_ | Type::kSecond))));
  }

  // atk_->GainExp(def_);

  bool is_last_attack = (reserve_second_attack == IsSecond());
  if (is_last_attack && !IsCounter() && def_->IsInRange(atk_->GetPosition())) {
    // Add counter attack command
    LOG_INFO("'%s's' counter-attack to '%s' is reserved.",
             def_->GetId().c_str(),
             atk_->GetId().c_str());
    ret->Append(unique_ptr<CmdBasicAttack>(new CmdBasicAttack(def_, atk_, CmdBasicAttack::Type::kCounter)));
  }
  return ret;
}

bool CmdBasicAttack::TryBasicAttack() {
  int chance = Formulae::ComputeBasicAttackAccuracy(atk_, def_);
  LOG_INFO("Chance of Hit : %d%", chance);
  return GenRandom(100) < chance;
}

bool CmdBasicAttack::TryBasicAttackCritical() {
  int chance = Formulae::ComputeBasicAttackCritical(atk_, def_);
  LOG_INFO("Chance of Critical : %d%", chance);
  return GenRandom(100) < chance;
}

bool CmdBasicAttack::TryBasicAttackDouble() {
  int chance = Formulae::ComputeBasicAttackDouble(atk_, def_);
  LOG_INFO("Chance of Double Attack : %d%", chance);
  return GenRandom(100) < chance;
}

int CmdBasicAttack::ComputeDamage(Map* map) {
  int damage = Formulae::ComputeBasicAttackDamage(map, atk_, def_);
  if (IsCounter()) damage = damage * 3 / 4;
  return damage;
}

// CmdMagic

CmdMagic::CmdMagic(Unit* atk, Unit* def, Magic* magic) : CmdAct(atk, def), magic_(magic) {
}

unique_ptr<Cmd> CmdMagic::Do(Game* game) {
  LOG_INFO("'%s' tries magic '%s' to '%s'",
           atk_->GetId().c_str(),
           magic_->GetId().c_str(),
           def_->GetId().c_str());
  bool hit = magic_->TryPerform(atk_, def_);
  Cmd* ret = nullptr;
  if (hit) {
    int damage = ComputeDamage(game->GetMap());
    ret = new CmdHit(atk_, def_, CmdActResult::Type::kMagic, CmdHit::HitType::kNormal, magic_, damage);
  } else {
    ret = new CmdMiss(atk_, def_, CmdActResult::Type::kMagic, magic_);
  }

  atk_->GainExp(def_);
  return unique_ptr<Cmd>(ret);
}

int CmdMagic::ComputeDamage(Map* map) {
  int damage = Formulae::ComputeMagicDamage(map, atk_, def_);
  return damage;
}

// CmdActResult

CmdActResult::CmdActResult(Unit* atk, Unit* def, Type type, Magic* magic)
    : CmdTwoUnits(atk, def),
      type_(type),
      magic_(magic) {
}

CmdActResult::CmdActResult(Unit* atk, Unit* def, Type type) : CmdActResult(atk, def, type, nullptr) {
  ASSERT(type == Type::kBasicAttack);
}

// CmdHit

CmdHit::CmdHit(Unit* atk, Unit* def, Type type, HitType hit_type, Magic* magic, int damage)
    : CmdActResult(atk, def, type, magic), hit_type_(hit_type), damage_(damage) {
}

CmdHit::CmdHit(Unit* atk, Unit* def, Type type, HitType hit_type, int damage)
    : CmdActResult(atk, def, type), hit_type_(hit_type), damage_(damage) {
}

unique_ptr<Cmd> CmdHit::Do(Game*) {
  unique_ptr<Cmd> ret = nullptr;
  if (type_ == Type::kBasicAttack) {
    const string hit_type = hit_type_ == HitType::kCritical ? "Critical" : "Normal";
    LOG_INFO("%s does damage to %s by %d (%s)",
             atk_->GetId().c_str(),
             def_->GetId().c_str(),
             damage_,
             hit_type.c_str());
    if (!def_->DoDamage(damage_)) { // unit is dead
      ret = unique_ptr<CmdKilled>(new CmdKilled(def_));
    }
  } else {
    ASSERT(type_ == Type::kMagic);
    magic_->Perform(atk_, def_);
  }
  return ret;
}

// CmdMiss

CmdMiss::CmdMiss(Unit* atk, Unit* def, Type type, Magic* magic) : CmdActResult(atk, def, type, magic) {
}

CmdMiss::CmdMiss(Unit* atk, Unit* def, Type type) : CmdActResult(atk, def, type) {
}

unique_ptr<Cmd> CmdMiss::Do(Game*) {
  LOG_INFO("%s misses!", atk_->GetId().c_str());
  return nullptr;
}

// CmdKilled

CmdKilled::CmdKilled(Unit* unit) : CmdOneUnit(unit) {
}

unique_ptr<Cmd> CmdKilled::Do(Game* game) {
  game->KillUnit(unit_);
  return nullptr;
}

// CmdMove

CmdMove::CmdMove(Unit* unit, Vec2D dest) : CmdOneUnit(unit), dest_(dest) {
}

unique_ptr<Cmd> CmdMove::Do(Game* game) {
  LOG_INFO("Unit '%s' moved from (%d, %d) to (%d, %d)",
           unit_->GetId().c_str(),
           unit_->GetPosition().x,
           unit_->GetPosition().y,
           dest_.x,
           dest_.y);
  game->MoveUnit(unit_, dest_);
  return nullptr;
}

// CmdAction

CmdAction::CmdAction() : CmdAction(Flag::kDecompose) {
}

CmdAction::CmdAction(Flag flag) : cmd_move_(nullptr), cmd_act_(nullptr), flag_(flag) {
}

void CmdAction::SetCmdMove(unique_ptr<CmdMove> cmd) {
  cmd_move_ = std::move(cmd);
}

void CmdAction::SetCmdAct(unique_ptr<CmdAct> cmd) {
  cmd_act_ = std::move(cmd);
}
unique_ptr<Cmd> CmdAction::Do(Game* game) {
  Unit* doer = cmd_act_ ? cmd_act_->GetUnitAtk() : cmd_move_->GetUnit();
  ASSERT(doer != nullptr);

  unique_ptr<CmdQueue> ret(new CmdQueue());

  if (flag_ == Flag::kNone) {
    if (cmd_move_ != nullptr) {
      unique_ptr<Cmd> tmp = cmd_move_->Do(game);
      ASSERT(tmp == nullptr);
    }

    if (cmd_act_ != nullptr) {
      unique_ptr<Cmd> tmp = cmd_act_->Do(game);
      ret->Append(std::move(tmp));
    }

    doer->EndAction();

    // TODO raise move event
    //      Even if it didn't move, we can just send message that we are at some position...
    //      BTW act event must have been notified by `cmd_act_`

    //  ret->Append(commands that were generated - raised by move event);
  } else {
    ASSERT(flag_ == Flag::kDecompose);
    LOG_DEBUG("DECOMPOSING Action");
    if (cmd_move_ != nullptr) {
      ret->Append(std::move(cmd_move_));
    }
    if (cmd_act_ != nullptr) {
      ret->Append(std::move(cmd_act_));
    }

    doer->EndAction();
  }

  return std::move(ret);
}

// CmdEndTurn

CmdEndTurn::CmdEndTurn() : Cmd() {
}

unique_ptr<Cmd> CmdEndTurn::Do(Game* game) {
  game->EndForceTurn();
  return nullptr;
}

// CmdPlayAI

CmdPlayAI::CmdPlayAI() : Cmd() {
}

unique_ptr<Cmd> CmdPlayAI::Do(Game* game) {
//  const Force force = turn_.GetForce();
  vector<Unit*> units = game->GetCurrentTurnUnits();
  LOG_DEBUG("units: %d", units.size());
  Unit* unit = nullptr;
  // Find first availible unit
  for (vector<Unit*>::iterator itr = units.begin(); itr != units.end(); itr++) {
    Unit* u = *itr;
    if (!u->IsDead() && !u->IsDoneAction()) {
      unit = u;
      break;
    }
  }

  // No more unit waiting for command
  if (unit == nullptr) {
    return unique_ptr<Cmd>(new CmdEndTurn());
  }

  // XXX create an AI handler module and delegate to it.
  // Currently a simple rushing AI is implemented here.
  CmdAction* cmd = new CmdAction(CmdAction::Flag::kDecompose);

//  cmd->SetCmdAct(new CmdStay(unit));

  unique_ptr<PathTree> movable_path(game->FindMovablePath(unit));
  vector<Vec2D> movable_pos_list = movable_path->GetNodeList();
  Vec2D move_pos = {-1, -1};
  Unit* target = nullptr;
  for (auto pos : movable_pos_list) {
    if (!game->UnitInCell(pos) || unit->GetPosition() == pos) {
      Unit* u = game->GetOneHostileInRange(unit, pos);
      if (u != nullptr) {
        move_pos = pos;
        target = u;
      }
    }
  }
  if (move_pos == Vec2D(-1, -1)) {
    int size = movable_pos_list.size();
    move_pos = movable_pos_list[GenRandom(size)];
  }
  cmd->SetCmdMove(unique_ptr<CmdMove>(new CmdMove(unit, move_pos)));
  if (target == nullptr) {
    cmd->SetCmdAct(unique_ptr<CmdStay>(new CmdStay(unit)));
  } else {
    cmd->SetCmdAct(unique_ptr<CmdBasicAttack>(new CmdBasicAttack(unit, target, CmdBasicAttack::Type::kActive)));
  }

  return unique_ptr<CmdAction>(cmd);
}

// CmdGameVictory

CmdGameVictory::CmdGameVictory() : Cmd() {
}

unique_ptr<Cmd> CmdGameVictory::Do(Game* game) {
  lua::Olua* lua = game->GetLuaScript();
  lua->Call<void>("on_victory", lua::LuaClass(game, "Game"));

  // Return a new CmdGameEnd just in case when user script does not specifies next scenario
  return unique_ptr<Cmd>(new CmdGameEnd(true));
}

// CmdGameEnd

CmdGameEnd::CmdGameEnd(bool is_victory) : Cmd(), is_victory_(is_victory) {
}

unique_ptr<Cmd> CmdGameEnd::Do(Game*) {
  // TODO Update Own units info
  return nullptr;
}

// CmdSpeak

CmdSpeak::CmdSpeak(Unit* unit, const string& words) : CmdOneUnit(unit), words_(words) {
}

unique_ptr<Cmd> CmdSpeak::Do(Game*) {
  return nullptr;
}

} // namespace core
} // namespace mengde
