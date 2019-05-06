#include "deserializer.h"
#include "scenario_generated.h"

#include <fstream>

namespace mengde {
namespace core {

Deserializer::Deserializer(const Path& path) : path_{path} {}

unique_ptr<Scenario> Deserializer::Deserialize() {
  std::ifstream file(path_.ToString(), std::ios::binary | std::ios::ate);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<char> buffer(size);
  file.read(buffer.data(), size);

  auto sce = save::GetScenario(buffer.data());

  auto assets = Build(*sce->resource_managers());

  (void)assets;

  return nullptr;
}

ResourceManagers Deserializer::Build(const save::ResourceManagers& rm) {
  ResourceManagers ret;
  ret.terrain_manager = Build(*rm.terrain_manager());
  ret.hero_class_manager = Build(*rm.hero_class_manager());
  return ret;
}

TerrainManager* Deserializer::Build(const flatbuffers::Vector<flatbuffers::Offset<save::Terrain>>& tm) {
  auto ret = new TerrainManager;
  for (uint32_t i = 0; i < tm.Length(); ++i) {
    auto t = tm.Get(i);
    auto id = t->id()->str();
    vector<int32_t> move_costs;
    for (auto cost : *t->move_costs()) {
      move_costs.push_back(cost);
    }
    vector<int32_t> effects;
    for (auto cost : *t->class_effects()) {
      effects.push_back(cost);
    }

    auto obj = new Terrain{id, move_costs, effects};
    obj->SetIndex(i);
    ret->Add(id, obj);
  }
  return ret;
}

HeroClassManager* Deserializer::Build(const flatbuffers::Vector<flatbuffers::Offset<save::HeroClass>>& hcm) {
  auto ret = new HeroClassManager;
  for (uint32_t i = 0; i < hcm.Length(); ++i) {
    auto hc = hcm.Get(i);
    auto id = hc->id()->str();
    auto stat_grade = BuildStruct<Attribute>(*hc->attr_grade());
    auto range = static_cast<Range::Type>(hc->attack_range());
    auto bi_hp = BuildStruct<BaseAndIncr>(*hc->bi_hp());
    auto bi_mp = BuildStruct<BaseAndIncr>(*hc->bi_mp());
    boost::optional<PromotionInfo> promotion_info;
    if (hc->promotion_info()) {
      promotion_info = PromotionInfo{hc->promotion_info()->hero_class_id()->str(), hc->promotion_info()->level()};
    }

    auto hc_obj = new HeroClass{id, i, stat_grade, range, hc->move(), bi_hp, bi_mp, promotion_info};
    ret->Add(id, hc_obj);
  }
  return ret;
}

}  // namespace core
}  // namespace mengde
