#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include <unordered_map>

#include "util/common.h"
#include "magic.h"
#include "equipment.h"
#include "unit_class.h"
#include "terrain.h"
#include "hero.h"

namespace mengde {
namespace core {

template<typename T>
class ResourceManager {
 public:
  ResourceManager() {
  }

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

  T* Get(const string& id) {
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

typedef ResourceManager<Magic>        MagicManager;
typedef ResourceManager<Equipment>    EquipmentManager;
typedef ResourceManager<UnitClass>    UnitClassManager;
typedef ResourceManager<Terrain>      TerrainManager;
typedef ResourceManager<HeroTemplate> HeroTemplateManager;

struct ResourceManagers {
  UnitClassManager*    unit_class_manager;
  TerrainManager*      terrain_manager;
  MagicManager*        magic_manager;
  EquipmentManager*    equipment_manager;
  HeroTemplateManager* hero_tpl_manager;

  ResourceManagers() :
      unit_class_manager(nullptr),
      terrain_manager(nullptr),
      magic_manager(nullptr),
      equipment_manager(nullptr),
      hero_tpl_manager(nullptr)
  {
  }
};

} // namespace core
} // namespace mengde

#endif // RESOURCE_MANAGER_H_
