#include "texture_animator.h"
#include "texture.h"

namespace mengde {
namespace gui {
namespace foundation {

TextureAnimator::TextureAnimator(Texture* texture, int fpc)
    : texture_(texture), cut_size_(0), num_cuts_(0), frames_per_cut_(fpc), frame_no_(0) {
  Vec2D texture_size = texture->size();
  cut_size_ = texture_size.x;
  num_cuts_ = texture_size.y / cut_size_;
  ASSERT(texture_size.y % texture_size.x == 0);
}

Rect TextureAnimator::GetCurrentCutRect() {
  int cur_cut = frame_no_ / frames_per_cut_;
  return {0, cur_cut * cut_size_, cut_size_, cut_size_};
}

void TextureAnimator::NextFrame() { frame_no_++; }

bool TextureAnimator::DoneAnimate() {
  int cur_cut = frame_no_ / frames_per_cut_;
  return cur_cut >= num_cuts_;
}

}  // namespace foundation
}  // namespace gui
}  // namespace mengde
