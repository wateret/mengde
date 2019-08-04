#ifndef MENGDE_CORE_CMDS_H_
#define MENGDE_CORE_CMDS_H_

#include "cmd.h"
#include "attribute_modifier.h"

namespace mengde {
namespace core {

class Unit;

//
// CmdInvalid : Unused, cannot be instantiated
//

class CmdInvalid : public Cmd {};

class CmdUnit : public Cmd {
 public:
  CmdUnit(const UId& uid);
  virtual unique_ptr<Cmd> Do(Stage*) override = 0;

 public:
  UId GetUnit() const { return unit_; }

 protected:
  UId unit_;
};

class CmdTwoUnits : public Cmd {
 public:
  CmdTwoUnits(const UId&, const UId&);
  virtual unique_ptr<Cmd> Do(Stage*) override = 0;

 public:
  UId GetUnitAtk() const { return atk_; }
  UId GetUnitDef() const { return def_; }
  void SwapAtkDef();

 protected:
  UId atk_;
  UId def_;
};

class CmdAct : public CmdTwoUnits {
 public:
  CmdAct(const UId&, const UId&);
  virtual unique_ptr<Cmd> Do(Stage*) override = 0;
};

class CmdEndAction : public CmdUnit {
 public:
  CmdEndAction(const UId&);
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdEndAction; }

 public:
  virtual void Accept(CmdVisitor& visitor) const override;
};

class CmdStay : public CmdAct {
 public:
  CmdStay(const UId&);
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdStay; }

 public:
  virtual void Accept(CmdVisitor& visitor) const override;
};

class CmdBasicAttack : public CmdAct {
 public:
  // Type of Basic Attack
  enum Type { kNone = 0x0, kActive = 0x1, kCounter = 0x2, kSecond = 0x4, kActiveOrCounter = kActive | kCounter };

 public:
  CmdBasicAttack(const UId&, const UId&, Type);
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdBasicAttack; }

 public:
  virtual void Accept(CmdVisitor& visitor) const override;

 public:
  Type type() const { return type_; }
  bool IsCounter() { return type_ & Type::kCounter; }
  bool IsSecond() { return type_ & Type::kSecond; }
  void UpdateChange(AttributeChange change) {
    change_.multiplier += change.multiplier;
    change_.addend += change.addend;
  }

 private:
  bool TryBasicAttack(Stage* stage);
  bool TryBasicAttackCritical(Stage* stage);
  bool TryBasicAttackDouble(Stage* stage);
  int ComputeDamage(Stage* stage, Map*);

 private:
  Type type_;
  AttributeChange change_;
};

class Magic;

class CmdMagic : public CmdAct {
 public:
  CmdMagic(const UId&, const UId&, Magic*);
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdMagic; }
  const Magic* magic() { return magic_; }

 public:
  virtual void Accept(CmdVisitor& visitor) const override;

 private:
  int ComputeDamage(Map*, const Unit*, const Unit*);

 private:
  Magic* magic_;
};

class CmdActResult : public CmdTwoUnits {
 public:
  enum class Type { kNone, kBasicAttack, kMagic };

 public:
  CmdActResult(const UId&, const UId&, Type, Magic*);
  CmdActResult(const UId&, const UId&, Type);
  virtual unique_ptr<Cmd> Do(Stage*) override = 0;

 public:
  bool IsBasicAttack() const { return type_ == Type::kBasicAttack; }
  bool IsMagic() const { return type_ == Type::kMagic; }
  Magic* GetMagic() const { /* ASSERT(IsMagic()); */
    return magic_;
  }

 protected:
  Type type_;
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
  CmdHit(const UId&, const UId&, Type, HitType, Magic*, int);
  CmdHit(const UId&, const UId&, Type, HitType, int);
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdHit; }

 public:
  virtual void Accept(CmdVisitor& visitor) const override;

 public:
  HitType GetHitType() const { return hit_type_; }
  int GetDamage() const { return damage_; }

 private:
  HitType hit_type_;
  int damage_;
};

class CmdMiss : public CmdActResult {
 public:
  CmdMiss(const UId&, const UId&, Type, Magic*);
  CmdMiss(const UId&, const UId&, Type);
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdMiss; }

 public:
  virtual void Accept(CmdVisitor& visitor) const override;
};

class CmdKilled : public CmdUnit {
 public:
  CmdKilled(const UId&);
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdKilled; }

 public:
  virtual void Accept(CmdVisitor& visitor) const override;
};

class CmdMove : public CmdUnit {
 public:
  CmdMove(const UId&, Vec2D);
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdMove; }
  Vec2D GetDest() const { return dest_; }

 public:
  virtual void Accept(CmdVisitor& visitor) const override;

 private:
  Vec2D dest_;
};

enum class ActionType {
  kNone,
  kStay,
  kBasicAttack,
  kMagic,
  // kUseConsumable
  kCount
};

class CmdAction : public Cmd {
 public:
  enum class Flag { kNone, kUserInput, kDecompose };

 public:
  CmdAction();
  CmdAction(Flag);
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdAction; }

 public:
  virtual void Accept(CmdVisitor& visitor) const override;

 public:
  void SetCmdMove(unique_ptr<CmdMove>);
  void SetCmdAct(unique_ptr<CmdAct>);

 private:
  unique_ptr<CmdMove> cmd_move_;
  unique_ptr<CmdAct> cmd_act_;
  Flag flag_;
};

class CmdEndTurn : public Cmd {
 public:
  CmdEndTurn();
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdEndTurn; }

 public:
  virtual void Accept(CmdVisitor& visitor) const override;
};

class CmdPlayAI : public Cmd {
 public:
  CmdPlayAI();
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdPlayAI; }

 public:
  virtual void Accept(CmdVisitor& visitor) const override;
};

class CmdGameVictory : public Cmd {
 public:
  CmdGameVictory();
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdGameVictory; }

 public:
  virtual void Accept(CmdVisitor& visitor) const override;
};

class CmdGameEnd : public Cmd {
 public:
  CmdGameEnd(bool is_victory);
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdGameEnd; }
  bool is_victory() const { return is_victory_; }

 public:
  virtual void Accept(CmdVisitor& visitor) const override;

 private:
  bool is_victory_;
};

class CmdSpeak : public CmdUnit {
 public:
  CmdSpeak(const UId&, const string&);
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdSpeak; }
  string GetWords() const { return words_; }

 public:
  virtual void Accept(CmdVisitor& visitor) const override;

 private:
  string words_;
};

class CmdRestoreHp : public CmdUnit {
 public:
  CmdRestoreHp(const UId&, AttributeChange change);
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdRestoreHp; }
  int CalcAmount(UserInterface* stage) const;

 public:
  virtual void Accept(CmdVisitor& visitor) const override;

 private:
  AttributeChange change_;
};

class CmdGainExp : public CmdUnit {
 public:
  CmdGainExp(const UId& unit, uint32_t exp);
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdGainExp; }
  uint32_t exp() const { return exp_; }

 public:
  virtual void Accept(CmdVisitor& visitor) const override;

 private:
  uint32_t exp_;
};

class CmdLevelUp : public CmdUnit {
 public:
  CmdLevelUp(const UId& unit);
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdLevelUp; }

 public:
  virtual void Accept(CmdVisitor& visitor) const override;
};

class CmdPromote : public CmdUnit {
 public:
  CmdPromote(const UId& unit);
  virtual unique_ptr<Cmd> Do(Stage*) override;
  virtual Cmd::Op op() const override { return Op::kCmdPromote; }

 public:
  virtual void Accept(CmdVisitor& visitor) const override;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_CMDS_H_
