#ifndef UTIL_STATE_MACHINE_H_
#define UTIL_STATE_MACHINE_H_

#include <stack>

// T can be only be a subclass of `State` and must be pointer type

template <typename T>
class StateMachine {
 public:
  StateMachine();
  StateMachine(T);
  ~StateMachine();
  T    GetCurrentState();
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
StateMachine<T>::StateMachine() {}

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
  while (!IsStackEmpty()) {
    PopState();
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
#ifdef DEBUG
  LOG_INFO("StateMachine Pushed   [%u] Top : %s", stack_.size(), GetCurrentState()->GetStateID().c_str());
#endif
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
#ifdef DEBUG
  LOG_INFO("StateMachine Replaced [%u] Top : %s", stack_.size(), GetCurrentState()->GetStateID().c_str());
#endif
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
#ifdef DEBUG
    LOG_INFO("StateMachine Popped   [%u] Top : %s", stack_.size(), GetCurrentState()->GetStateID().c_str());
#endif
  } else {
#ifdef DEBUG
    LOG_INFO("StateMachine Popped   [0]");
#endif
  }
}

#endif  // UTIL_STATE_MACHINE_H_
