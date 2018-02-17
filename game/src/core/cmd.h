#ifndef COMMAND_H_
#define COMMAND_H_

#include "unit.h"
#include "util/common.h"

namespace mengde {
namespace core {

class Game;
class Map;

extern const char* kCmdOpToString[];

class Cmd {
 public:
  enum class Op {
#define MACRO_CMD_OP(name) kCmd##name,
#include "cmd_op.h.inc"
  };

 public:
  virtual ~Cmd()                    = default;
  virtual unique_ptr<Cmd> Do(Game*) = 0;
  // virtual Cmd* Undo(Game*) = 0;
  virtual Cmd::Op GetOp() const = 0;

#ifdef DEBUG
  //  static string OpToString(Op op) { return kCmdOpToString[static_cast<int>(op)]; }
  virtual void DebugPrint() const { printf("%s\n", kCmdOpToString[static_cast<int>(GetOp())]); }
#endif
};

// CmdQueue : command queue
class CmdQueue : public Cmd {
 public:
  CmdQueue();
  ~CmdQueue();
  virtual unique_ptr<Cmd> Do(Game*) override;
  virtual Cmd::Op         GetOp() const override { return Op::kCmdQueue; }

#ifdef DEBUG
  virtual void DebugPrint() const override;
#endif

 public:
  void       Prepend(unique_ptr<Cmd>);
  void       Append(unique_ptr<Cmd>);
  bool       IsEmpty() const;
  const Cmd* GetNextCmdConst() const;

  CmdQueue& operator+=(unique_ptr<Cmd>);

 private:
  void Insert(unique_ptr<Cmd>, bool prepend);

 private:
  deque<unique_ptr<Cmd>> q_;
};

// CmdQueue operators

class CmdUnit : public Cmd {
 public:
  CmdUnit(Unit*);
  virtual unique_ptr<Cmd> Do(Game*) override = 0;

 public:
  Unit* GetUnit() const { return unit_; }

 protected:
  Unit* unit_;
};

class CmdTwoUnits : public Cmd {
 public:
  CmdTwoUnits(Unit*, Unit*);
  virtual unique_ptr<Cmd> Do(Game*) override = 0;

#ifdef DEBUG
  virtual void DebugPrint() const override;
#endif

 public:
  Unit* GetUnitAtk() const { return atk_; }
  Unit* GetUnitDef() const { return def_; }
  void  SwapAtkDef();

 protected:
  Unit* atk_;
  Unit* def_;
};

class CmdAct : public CmdTwoUnits {
 public:
  CmdAct(Unit*, Unit*);
  virtual unique_ptr<Cmd> Do(Game*) override = 0;
};

class CmdEndAction : public CmdUnit {
 public:
  CmdEndAction(Unit*);
  virtual unique_ptr<Cmd> Do(Game*) override;
  virtual Cmd::Op         GetOp() const override { return Op::kCmdEndAction; }
};

class CmdStay : public CmdAct {
 public:
  CmdStay(Unit*);
  virtual unique_ptr<Cmd> Do(Game*) override;
  virtual Cmd::Op         GetOp() const override { return Op::kCmdStay; }
};

class CmdBasicAttack : public CmdAct {
 public:
  // Type of Basic Attack
  enum Type { kNone = 0x0, kActive = 0x1, kCounter = 0x2, kSecond = 0x4, kActiveOrCounter = kActive | kCounter };

 public:
  CmdBasicAttack(Unit*, Unit*, Type);
  virtual unique_ptr<Cmd> Do(Game*) override;
  virtual Cmd::Op         GetOp() const override { return Op::kCmdBasicAttack; }

 public:
  bool IsCounter() { return type_ & Type::kCounter; }
  bool IsSecond() { return type_ & Type::kSecond; }
  void AddToMultiplier(int m) { multiplier_ += m; }
  void AddToAddend(int m) { addend_ += m; }

 private:
  bool TryBasicAttack();
  bool TryBasicAttackCritical();
  bool TryBasicAttackDouble();
  int  ComputeDamage(Map*);

 private:
  Type type_;
  int  multiplier_;
  int  addend_;
};

class Magic;

class CmdMagic : public CmdAct {
 public:
  CmdMagic(Unit*, Unit*, Magic*);
  virtual unique_ptr<Cmd> Do(Game*) override;
  virtual Cmd::Op         GetOp() const override { return Op::kCmdMagic; }

 private:
  int ComputeDamage(Map*);

 private:
  Magic* magic_;
};

class CmdActResult : public CmdTwoUnits {
 public:
  enum class Type { kNone, kBasicAttack, kMagic };

 public:
  CmdActResult(Unit*, Unit*, Type, Magic*);
  CmdActResult(Unit*, Unit*, Type);
  virtual unique_ptr<Cmd> Do(Game*) override = 0;

 public:
  bool   IsBasicAttack() const { return type_ == Type::kBasicAttack; }
  bool   IsMagic() const { return type_ == Type::kMagic; }
  Magic* GetMagic() const { /* ASSERT(IsMagic()); */
    return magic_;
  }

 protected:
  Type   type_;
  Magic* magic_;
};

class CmdHit : public CmdActResult {
 public:
  enum class HitType {
    kNone,
    kNormal,
    kCritical,
  };

 public:
  CmdHit(Unit*, Unit*, Type, HitType, Magic*, int);
  CmdHit(Unit*, Unit*, Type, HitType, int);
  virtual unique_ptr<Cmd> Do(Game*) override;
  virtual Cmd::Op         GetOp() const override { return Op::kCmdHit; }

 public:
  HitType GetHitType() const { return hit_type_; }
  int     GetDamage() const { return damage_; }

 private:
  HitType hit_type_;
  int     damage_;
};

class CmdMiss : public CmdActResult {
 public:
  CmdMiss(Unit*, Unit*, Type, Magic*);
  CmdMiss(Unit*, Unit*, Type);
  virtual unique_ptr<Cmd> Do(Game*) override;
  virtual Cmd::Op         GetOp() const override { return Op::kCmdMiss; }
};

class CmdKilled : public CmdUnit {
 public:
  CmdKilled(Unit*);
  virtual unique_ptr<Cmd> Do(Game*) override;
  virtual Cmd::Op         GetOp() const override { return Op::kCmdKilled; }
};

class CmdMove : public CmdUnit {
 public:
  CmdMove(Unit*, Vec2D);
  virtual unique_ptr<Cmd> Do(Game*) override;
  virtual Cmd::Op         GetOp() const override { return Op::kCmdMove; }
  Vec2D                   GetDest() const { return dest_; }

 private:
  Vec2D dest_;
};

class CmdAction : public Cmd {
 public:
  enum class Flag { kNone, kDecompose };

 public:
  CmdAction();
  CmdAction(Flag);
  virtual unique_ptr<Cmd> Do(Game*) override;
  virtual Cmd::Op         GetOp() const override { return Op::kCmdAction; }

 public:
  void SetCmdMove(unique_ptr<CmdMove>);
  void SetCmdAct(unique_ptr<CmdAct>);

 private:
  unique_ptr<CmdMove> cmd_move_;
  unique_ptr<CmdAct>  cmd_act_;
  Flag                flag_;
};

class CmdEndTurn : public Cmd {
 public:
  CmdEndTurn();
  virtual unique_ptr<Cmd> Do(Game*) override;
  virtual Cmd::Op         GetOp() const override { return Op::kCmdEndTurn; }
};

class CmdPlayAI : public Cmd {
 public:
  CmdPlayAI();
  virtual unique_ptr<Cmd> Do(Game*) override;
  virtual Cmd::Op         GetOp() const override { return Op::kCmdPlayAI; }
};

class CmdGameVictory : public Cmd {
 public:
  CmdGameVictory();
  virtual unique_ptr<Cmd> Do(Game*) override;
  virtual Cmd::Op         GetOp() const override { return Op::kCmdGameVictory; }
};

class CmdGameEnd : public Cmd {
 public:
  CmdGameEnd(bool is_victory);
  virtual unique_ptr<Cmd> Do(Game*) override;
  virtual Cmd::Op         GetOp() const override { return Op::kCmdGameEnd; }
  bool                    is_victory() const { return is_victory_; }

 private:
  bool is_victory_;
};

class CmdSpeak : public CmdUnit {
 public:
  CmdSpeak(Unit*, const string&);
  virtual unique_ptr<Cmd> Do(Game*) override;
  virtual Cmd::Op         GetOp() const override { return Op::kCmdSpeak; }
  string                  GetWords() const { return words_; }

 private:
  string words_;
};

class CmdRestoreHp : public CmdUnit {
 public:
  CmdRestoreHp(Unit*, int ratio, int adder);
  virtual unique_ptr<Cmd> Do(Game*) override;
  virtual Cmd::Op         GetOp() const override { return Op::kCmdRestoreHp; }
  int                     CalcAmount() const;

 private:
  int ratio_;
  int adder_;
};

}  // namespace core
}  // namespace mengde

#endif  // COMMAND_H_
