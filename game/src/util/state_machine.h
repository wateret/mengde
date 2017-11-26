#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include <stack>

// T can be only be a subclass of `State` and must be pointer type

template <typename T>
class StateMachine {
 public:
  StateMachine(T);
  ~StateMachine();
  T GetCurrentState();
  void PushState(T);
  void ChangeState(T);
  void PopState();
  void InitState();

 private:
  bool IsStackEmpty() { return stack_.empty(); }

 private:
  std::stack<T> stack_;
};

template <typename T>
StateMachine<T>::StateMachine(T state) {
  PushState(state);
}

template <typename T>
StateMachine<T>::~StateMachine() {
  ASSERT(!IsStackEmpty());
  while (!stack_.empty()) {
    PopState();
  }
}

template <typename T>
void StateMachine<T>::InitState() {
  if (stack_.size() > 1) {
    GetCurrentState()->Exit();
    while (stack_.size() > 1) {
      T cur_state = GetCurrentState();
      delete cur_state;
      stack_.pop();
    }
    GetCurrentState()->Enter();
  }
}

template <typename T>
T StateMachine<T>::GetCurrentState() {
  ASSERT(!IsStackEmpty());
  return stack_.top();
}

template <typename T>
void StateMachine<T>::PushState(T state) {
  if (!IsStackEmpty()) {
    GetCurrentState()->Exit();
  }
  stack_.push(state);
//  LOG_INFO("StateMachine Pushed   / CurrentState : %s, StackSize: %d", GetCurrentState()->GetStateID().c_str(), stack_.size());
  state->Enter();
}

template <typename T>
void StateMachine<T>::ChangeState(T state) {
  ASSERT(!IsStackEmpty());
  T cur_state = GetCurrentState();
  cur_state->Exit();
  delete cur_state;
  stack_.pop();
  stack_.push(state);
  state->Enter();
//  LOG_INFO("StateMachine Replaced / CurrentState : %s, StackSize: %d", GetCurrentState()->GetStateID().c_str(), stack_.size());
}

template <typename T>
void StateMachine<T>::PopState() {
  ASSERT(!IsStackEmpty());
  T cur_state = GetCurrentState();
  cur_state->Exit();
  delete cur_state;
  stack_.pop();
  if (!IsStackEmpty()) {
    cur_state = GetCurrentState();
    cur_state->Enter();
//    LOG_INFO("StateMachine Popped   / CurrentState : %s, StackSize: %d", GetCurrentState()->GetStateID().c_str(), stack_.size());
  }
  else {
//    LOG_INFO("StateMachine Popped   / Stack is empty");
  }
}

#endif // STATE_MACHINE_H_
