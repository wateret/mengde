#ifndef I_VIEW_H_
#define I_VIEW_H_

#include "drawer.h"

class IView {
 public:
  virtual ~IView() {}
  virtual void Render(Drawer*) = 0;
  virtual void Update() = 0;

 private:
};

#endif // I_VIEW_H_
