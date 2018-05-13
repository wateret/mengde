#include "color.h"
#include "util/logger.h"

namespace mengde {
namespace gui {
namespace foundation {

ColorGen::ColorGen() {
  colors_.insert(std::make_pair("transparent", Color(255, 255, 255, 0)));
  colors_.insert(std::make_pair("white", Color(255, 255, 255)));
  colors_.insert(std::make_pair("black", Color(0, 0, 0)));
  colors_.insert(std::make_pair("red", Color(255, 0, 0)));
  colors_.insert(std::make_pair("green", Color(0, 255, 0)));
  colors_.insert(std::make_pair("blue", Color(0, 0, 255)));
  colors_.insert(std::make_pair("gray", Color(128, 128, 128)));
  colors_.insert(std::make_pair("darkgray", Color(64, 64, 64)));
  colors_.insert(std::make_pair("yellow", Color(255, 255, 0)));
  colors_.insert(std::make_pair("lightyellow", Color(255, 255, 224)));
  colors_.insert(std::make_pair("navy", Color(0, 0, 128)));
  colors_.insert(std::make_pair("orange", Color(255, 165, 0)));

  colors_.insert(std::make_pair("gauge_stats", Color(255, 203, 107)));
  colors_.insert(std::make_pair("gauge_bg", Color(128, 128, 128)));
  colors_.insert(std::make_pair("gauge_hp", Color(32, 160, 32)));
  colors_.insert(std::make_pair("gauge_mp", Color(48, 160, 192)));
  colors_.insert(std::make_pair("gauge_exp", Color(199, 146, 234)));
  colors_.insert(std::make_pair("gauge_hp_damage", Color(192, 64, 64)));
}

ColorGen* ColorGen::GetInstance() {
  static ColorGen color_gen;
  return &color_gen;
}

Color ColorGen::Generate(const std::string& name) {
  auto elem = colors_.find(name);
  if (elem == colors_.end()) {
    LOG_ERROR("Color name '%s' is not registered", name.c_str());
    return {0, 0, 0, 255};  // return black
  } else {
    Color color = elem->second;
    return color;
  }
}

Color ColorGen::Generate(const std::string& name, uint8_t alpha) {
  Color color = Generate(name);
  color.a = alpha;
  return color;
}

Color ColorGen::Generate(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha) { return Color(r, g, b, alpha); }

}  // namespace foundation
}  // namespace gui
}  // namespace mengde
