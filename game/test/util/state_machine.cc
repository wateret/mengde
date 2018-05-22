#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

#include "util/state.h"
#include "util/state_machine.h"

class TestState : public State {
 public:
  TestState() : enter_count_{0}, exit_count_{0} {}
  virtual void Enter() override { enter_count_++; }
  virtual void Exit() override { exit_count_++; }
#ifdef DEBUG
  virtual std::string GetStateID() const override { return "TestState"; }
#endif

  int enter_count() const { return enter_count_; }
  int exit_count() const { return exit_count_; }

 private:
  int enter_count_;
  int exit_count_;
};

BOOST_AUTO_TEST_CASE(EnterExit1) {
  StateMachine<TestState*> sm;
  sm.PushState(new TestState);

  auto state = sm.GetCurrentState();
  BOOST_CHECK(state->enter_count() == 1);
  BOOST_CHECK(state->exit_count() == 0);
}

BOOST_AUTO_TEST_CASE(EnterExit2) {
  StateMachine<TestState*> sm;
  sm.PushState(new TestState);
  sm.PushState(new TestState);
  sm.PopState();
  sm.PushState(new TestState);
  sm.PopState();

  auto state = sm.GetCurrentState();
  BOOST_CHECK(state->enter_count() == 3);
  BOOST_CHECK(state->exit_count() == 2);
}
