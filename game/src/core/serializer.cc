#include "serializer.h"

#include <fstream>

namespace mengde {
namespace core {

Serializer::Serializer(const Path& path) : path_{path} {}

void Serializer::Serialize(const Scenario& scenario) {
  builder_ = flatbuffers::FlatBufferBuilder{1024};

  auto sce = Build(scenario);

  builder_.Finish(sce);

  uint8_t* buffer = builder_.GetBufferPointer();
  uint32_t size = builder_.GetSize();

  std::ofstream file(path_.ToString(), std::ios::binary);
  file.write(reinterpret_cast<const char*>(buffer), size);
}

flatbuffers::Offset<save::Scenario> Serializer::Build(const Scenario& scenario) {
  auto id_off = builder_.CreateString(scenario.id());
  auto string_offset_vec = builder_.CreateVectorOfStrings(scenario.stage_id_list());
  auto resource_managers_off = Build(scenario.GetResourceManagers());

  save::ScenarioBuilder sce_builder(builder_);

  sce_builder.add_id(id_off);
  sce_builder.add_stage_id_list(string_offset_vec);
  sce_builder.add_stage_no(scenario.stage_no());
  sce_builder.add_resource_managers(resource_managers_off);

  return sce_builder.Finish();
}

flatbuffers::Offset<save::ResourceManagers> Serializer::Build(const ResourceManagers& rm) {
  auto tm = Build(*rm.terrain_manager);
  auto hcm = Build(*rm.hero_class_manager);

  std::vector<flatbuffers::Offset<save::Magic>> magic_vec;
  rm.magic_manager->ForEach([&](const string&, const Magic& magic) { magic_vec.push_back(Build(magic)); });
  auto mm = builder_.CreateVector(magic_vec);

  auto em = Build(*rm.equipment_manager);
  auto htm = Build(*rm.hero_tpl_manager);

  return save::CreateResourceManagers(builder_, tm, hcm, mm, em, htm);
}

flatbuffers::Offset<save::HeroClassManager> Serializer::Build(const HeroClassManager& hcm) {
  std::vector<flatbuffers::Offset<save::HeroClass>> records;
  hcm.ForEach([&](const string& id, const HeroClass& hero_class) {
    ASSERT(id == hero_class.id());
    records.push_back(Build(hero_class));
  });
  return save::CreateHeroClassManager(builder_, builder_.CreateVector(records));
}

flatbuffers::Offset<save::HeroTemplateManager> Serializer::Build(const HeroTemplateManager& htm) {
  std::vector<flatbuffers::Offset<save::HeroTemplate>> records;
  htm.ForEach([&](const string& id, const HeroTemplate& hero_tpl) {
    ASSERT(id == hero_tpl.id());
    records.push_back(Build(hero_tpl));
  });
  return save::CreateHeroTemplateManager(builder_, builder_.CreateVector(records));
}

flatbuffers::Offset<save::HeroClass> Serializer::Build(const HeroClass& hero_class) {
  auto id_off = builder_.CreateString(hero_class.id());
  auto attr_inl = BuildStruct<save::Attribute>(hero_class.stat_grade());
  auto attack_range = static_cast<int>(hero_class.attack_range_enum());
  auto move = hero_class.move();
  auto hp_inl = BuildStruct<save::BaseIncr>(hero_class.bni_hp());
  auto mp_inl = BuildStruct<save::BaseIncr>(hero_class.bni_mp());
  auto pi_off = hero_class.promotion_info() ? Build(*hero_class.promotion_info()) : 0;
  return save::CreateHeroClass(builder_, id_off, attr_inl, attack_range, move, hp_inl, mp_inl, pi_off);
}

flatbuffers::Offset<save::HeroTemplate> Serializer::Build(const HeroTemplate& hero_tpl) {
  auto id_off = builder_.CreateString(hero_tpl.id());
  auto hero_class_off = builder_.CreateString(hero_tpl.hero_class()->id());
  auto attr_inl = BuildStruct<save::Attribute>(hero_tpl.GetHeroStat());
  return save::CreateHeroTemplate(builder_, id_off, hero_class_off, attr_inl);
}

flatbuffers::Offset<save::TerrainManager> Serializer::Build(const TerrainManager& tm) {
  std::vector<flatbuffers::Offset<save::TerrainRecord>> terrain_records;
  tm.ForEach([&](const string& id, const Terrain& terrain) { terrain_records.push_back(Build(id, terrain)); });
  auto terrain_records_off = builder_.CreateVector(terrain_records);
  return save::CreateTerrainManager(builder_, terrain_records_off);
}

flatbuffers::Offset<save::TerrainRecord> Serializer::Build(const string& id, const Terrain& terrain) {
  auto id_off = builder_.CreateString(id);
  auto terrain_off = Build(terrain);
  return save::CreateTerrainRecord(builder_, id_off, terrain_off);
}

flatbuffers::Offset<save::Terrain> Serializer::Build(const Terrain& terrain) {
  return save::CreateTerrainDirect(builder_, terrain.id().c_str(), &terrain.move_costs(), &terrain.class_effects());
}

flatbuffers::Offset<save::PromotionInfo> Serializer::Build(const PromotionInfo& promotion_info) {
  return save::CreatePromotionInfoDirect(builder_, promotion_info.id.c_str(), promotion_info.level);
}

flatbuffers::Offset<save::EquipmentManager> Serializer::Build(const EquipmentManager& em) {
  std::vector<flatbuffers::Offset<save::Equipment>> records;
  em.ForEach([&](const string& id, const Equipment& equipment) {
    ASSERT(id == equipment.GetId());
    records.push_back(Build(equipment));
  });
  return save::CreateEquipmentManager(builder_, builder_.CreateVector(records));
}

flatbuffers::Offset<save::Equipment> Serializer::Build(const Equipment& equipment) {
  auto id_off = builder_.CreateString(equipment.GetId());
  auto type = static_cast<int>(equipment.GetType());
  auto va_off = Build(equipment.volatile_attribute());
  return save::CreateEquipment(builder_, id_off, type, va_off);
}

flatbuffers::Offset<save::VolatileAttributes> Serializer::Build(const VolatileAttribute& va) {
  std::vector<flatbuffers::Offset<save::AttributeModifier>> am_vec;
  va.stat_modifier_list().iterate([&](const StatModifier& am) { am_vec.push_back(Build(am)); });
  auto aml = builder_.CreateVector(am_vec);

  std::vector<flatbuffers::Offset<save::EventEffect>> ee_vec;
  va.event_effect_list().iterate([&](const EventEffectBase& ee) { ee_vec.push_back(Build(ee)); });
  auto eel = builder_.CreateVector(ee_vec);

  return save::CreateVolatileAttributes(builder_, aml, eel);
}

flatbuffers::Offset<save::AttributeModifier> Serializer::Build(const StatModifier& am) {
  return save::CreateAttributeModifierDirect(builder_, am.id().c_str(), am.stat_id(),
                                             BuildStruct<save::TurnBased>(am.turn()),
                                             BuildStruct<save::StatMod>(am.mod()));
}

flatbuffers::Offset<save::EventEffect> Serializer::Build(const EventEffectBase& event_effect) {
  save::EventEffectImpl ee_type = save::EventEffectImpl::NONE;
  flatbuffers::Offset<void> ee_inst = 0;
  auto gee = dynamic_cast<const GeneralEventEffect*>(&event_effect);
  auto ocee = dynamic_cast<const OnCmdEventEffect*>(&event_effect);
  if (gee) {
    ee_type = save::EventEffectImpl::GeneralEventEffect;
    ee_inst = Build(*gee).Union();
  } else {
    assert(ocee);
    ee_type = save::EventEffectImpl::OnCmdEventEffect;
    ee_inst = Build(*ocee).Union();
  }

  auto turn_inl = BuildStruct<save::TurnBased>(event_effect.turn());
  return save::CreateEventEffect(builder_, turn_inl, ee_type, ee_inst);
}

flatbuffers::Offset<save::GeneralEventEffect> Serializer::Build(const GeneralEventEffect& gee) {
  save::GeneralEventEffectImpl gee_type = save::GeneralEventEffectImpl::NONE;
  flatbuffers::Offset<void> gee_inst = 0;
  auto restore_hp = dynamic_cast<const GEERestoreHp*>(&gee);
  if (restore_hp) {
    gee_type = save::GeneralEventEffectImpl::GEERestoreHp;
    gee_inst = Build(*restore_hp).Union();
  }

  auto type = static_cast<int>(gee.type());
  return save::CreateGeneralEventEffect(builder_, type, gee_type, gee_inst);
}

flatbuffers::Offset<save::GEERestoreHp> Serializer::Build(const GEERestoreHp& gee_restore_hp) {
  auto sm = gee_restore_hp.stat_mod();
  return save::CreateGEERestoreHp(builder_, BuildStruct<save::StatMod>(sm));
}

flatbuffers::Offset<save::OnCmdEventEffect> Serializer::Build(const OnCmdEventEffect& ocee) {
  save::OnCmdEventEffectImpl ocee_type = save::OnCmdEventEffectImpl::NONE;
  flatbuffers::Offset<void> ocee_inst = 0;
  auto preemptive_attack = dynamic_cast<const OCEEPreemptiveAttack*>(&ocee);
  auto enhance_basic_attack = dynamic_cast<const OCEEEnhanceBasicAttack*>(&ocee);
  if (preemptive_attack) {
    ocee_type = save::OnCmdEventEffectImpl::OCEEPreemptiveAttack;
    ocee_inst = Build(*preemptive_attack).Union();
  } else if (enhance_basic_attack) {
    ocee_type = save::OnCmdEventEffectImpl::OCEEEnhanceBasicAttack;
    ocee_inst = Build(*enhance_basic_attack).Union();
  }

  auto type = static_cast<int>(ocee.type());
  return save::CreateOnCmdEventEffect(builder_, type, ocee_type, ocee_inst);
}

flatbuffers::Offset<save::OCEEPreemptiveAttack> Serializer::Build(const OCEEPreemptiveAttack&) {
  return save::CreateOCEEPreemptiveAttack(builder_);
}

flatbuffers::Offset<save::OCEEEnhanceBasicAttack> Serializer::Build(const OCEEEnhanceBasicAttack& obj) {
  auto sm = obj.stat_mod();
  return save::CreateOCEEEnhanceBasicAttack(builder_, BuildStruct<save::StatMod>(sm));
}

flatbuffers::Offset<save::Magic> Serializer::Build(const Magic& magic) {
  auto id = builder_.CreateString(magic.GetId());
  auto range = static_cast<int>(magic.range_enum());
  std::vector<const save::LearnInfo*> li_vec;
  for (const auto& li : magic.learn_info_list()) {
    li_vec.push_back(BuildStruct<save::LearnInfo>(li));
  }
  auto li_off = builder_.CreateVector(li_vec);
  auto target_enemy = magic.is_target_enemy();
  auto mp_cost = magic.mp_cost();
  std::vector<flatbuffers::Offset<save::MagicEffect>> eff_vec;
  for (const auto& itr : magic.effects()) {
    eff_vec.push_back(Build(*itr.second));
  }
  auto effects_off = builder_.CreateVector(eff_vec);
  return save::CreateMagic(builder_, id, range, li_off, target_enemy, mp_cost, effects_off);
}

flatbuffers::Offset<save::MagicEffect> Serializer::Build(const MagicEffect& magic_effect) {
  auto core_me_type = static_cast<int>(magic_effect.type());
  save::MagicEffectImpl me_type = save::MagicEffectImpl::NONE;
  flatbuffers::Offset<void> me_inst = 0;
  auto hp = dynamic_cast<const MagicEffectHP*>(&magic_effect);
  auto attribute = dynamic_cast<const MagicEffectStat*>(&magic_effect);
  auto condition = dynamic_cast<const MagicEffectCondition*>(&magic_effect);
  if (hp) {
    me_type = save::MagicEffectImpl::MagicEffectHp;
    me_inst = Build(*hp).Union();
  } else if (attribute) {
    me_type = save::MagicEffectImpl::MagicEffectAttribute;
    me_inst = Build(*attribute).Union();
  } else if (condition) {
    me_type = save::MagicEffectImpl::MagicEffectCondition;
    me_inst = Build(*condition).Union();
  } else {
    ASSERT("Cannot serialize Unknown MagicEffect");
  }
  return save::CreateMagicEffect(builder_, core_me_type, me_type, me_inst);
}

flatbuffers::Offset<save::MagicEffectHp> Serializer::Build(const MagicEffectHP& me_hp) {
  return save::CreateMagicEffectHp(builder_, me_hp.power());
}

flatbuffers::Offset<save::MagicEffectAttribute> Serializer::Build(const MagicEffectStat& me_attr) {
  return save::CreateMagicEffectAttribute(builder_, Build(me_attr.attribute_modifier()));
}

flatbuffers::Offset<save::MagicEffectCondition> Serializer::Build(const MagicEffectCondition& me_cond) {
  return save::CreateMagicEffectCondition(builder_, static_cast<int>(me_cond.condition()),
                                          BuildStruct<save::TurnBased>(me_cond.turn()));
}

}  // namespace core
}  // namespace mengde

#if 0
void Serializer::Deserialize() {
  std::ifstream file(path_.ToString(), std::ios::binary | std::ios::ate);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<char> buffer(size);
  file.read(buffer.data(), size);

  auto sce = save::GetScenario(buffer.data());
  auto val = sce->id();
  (void)val;
}
#endif
