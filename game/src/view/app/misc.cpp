#include "misc.h"

std::string GetModelPath(const std::string& model_id, SpriteType type) {
  return "model/" + model_id + "/" + kSpriteBitmapName[type] + ".bmp";
}
