#ifndef I_DEPLOY_HELPER_H_
#define I_DEPLOY_HELPER_H_

#include "util/common.h"

class Hero;

class IDeployHelper {
 public:
  virtual void     SubmitDeploy() = 0;
  virtual uint32_t AssignDeploy(const shared_ptr<Hero>&) = 0;
  virtual uint32_t UnassignDeploy(const shared_ptr<Hero>&) = 0;
  virtual uint32_t FindDeploy(const shared_ptr<Hero>&) = 0;
};

#endif // I_DEPLOY_HELPER_H_
