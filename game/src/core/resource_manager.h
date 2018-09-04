#ifndef MENGDE_CORE_RESOURCE_MANAGER_H_
#define MENGDE_CORE_RESOURCE_MANAGER_H_

#include <unordered_map>

#include "equipment.h"
#include "hero_template.h"
#include "magic.h"
#include "terrain.h"
#include "unit_class.h"
#include "util/common.h"

namespace mengde {
namespace core {

template <typename T>
class ResourceManager {
 public:
  ResourceManager() {}

  ~ResourceManager() {
    for (auto e : container_) {
      ASSERT(e.second != nullptr);
      if (e.second != nullptr) {
        delete e.second;
      }
    }
  }

  void Add(const string& id, T* e) {
    ASSERT(container_.find(id) == container_.end());
    container_[id] = e;
  }

  T* Get(const string& id) { return const_cast<T*>(static_cast<const ResourceManager*>(this)->Get(id)); }

  const T* Get(const string& id) const {
    auto iter = container_.find(id);
    if (iter != container_.end()) {
      return iter->second;
    } else {
      LOG_ERROR("Element for ID '%s' does not exist.", id.c_str());
      UNREACHABLE("Element for the given ID does not exist.");
      return nullptr;
    }
  }

  uint32_t GetNumElements() { return container_.size(); }

  void ForEach(function<void(T*)> f) {
    for (auto e : container_) {
      f(e.second);
    }
  }

 private:
  std::unordered_map<string, T*> container_;
};

using MagicManager = ResourceManager<Magic>;
using EquipmentManager = ResourceManager<Equipment>;
using UnitClassManager = ResourceManager<UnitClass>;
using TerrainManager = ResourceManager<Terrain>;
using HeroTemplateManager = ResourceManager<HeroTemplate>;

struct ResourceManagers {
  UnitClassManager* unit_class_manager;
  TerrainManager* terrain_manager;
  MagicManager* magic_manager;
  EquipmentManager* equipment_manager;
  HeroTemplateManager* hero_tpl_manager;

  ResourceManagers()
      : unit_class_manager(nullptr),
        terrain_manager(nullptr),
        magic_manager(nullptr),
        equipment_manager(nullptr),
        hero_tpl_manager(nullptr) {}
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_RESOURCE_MANAGER_H_
