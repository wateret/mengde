#ifndef DEPLOY_VIEW_H_
#define DEPLOY_VIEW_H_

#include "composite_view.h"

class Hero;

class HeroModelView : public CompositeView {
 public:
  HeroModelView(const Rect&, shared_ptr<Hero>);

 private:
  shared_ptr<Hero> hero_;
};

class HeroModelListView : public CompositeView {
 public:
  HeroModelListView(const Rect&, const vector<shared_ptr<Hero>>&);

 private:
};

// DeployView is a UI view for deploying heroes

class DeployView : public CompositeView {
 public:
  DeployView(const Rect&, const vector<shared_ptr<Hero>>&);

 private:
  vector<shared_ptr<Hero>> hero_list_;
};

#endif // DEPLOY_VIEW_H_
