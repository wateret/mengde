#ifndef TEXTURE_ANIMATOR_H_
#define TEXTURE_ANIMATOR_H_

#include "common.h"
#include "rect.h"

class Texture;

class TextureAnimator {
 public:
  TextureAnimator(Texture*, int);
  Rect GetCurrentCutRect();
  void NextFrame();
  bool DoneAnimate();
  Texture* GetTexture() { return texture_; }
  int GetCutSize() { return cut_size_; }

 private:
  Texture* texture_;
  int      cut_size_;
  int      num_cuts_;
  int      frames_per_cut_;
  int      frame_no_;
};

#endif // TEXTURE_ANIMATOR_H_
