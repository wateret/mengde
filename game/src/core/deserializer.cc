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
  ret.hero_class_manager = Build(*rm.hero_class_manager());
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
