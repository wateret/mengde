#ifndef MENGDE_CORE_I_DEPLOY_HELPER_H_
#define MENGDE_CORE_I_DEPLOY_HELPER_H_

#include "util/common.h"

namespace mengde {
namespace core {

class Hero;

class IDeployHelper {
 public:
  virtual ~IDeployHelper() = default;
  virtual bool SubmitDeploy() = 0;
  virtual uint32_t AssignDeploy(const Hero*) = 0;
  virtual uint32_t UnassignDeploy(const Hero*) = 0;
  virtual uint32_t FindDeploy(const Hero*) = 0;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_I_DEPLOY_HELPER_H_
