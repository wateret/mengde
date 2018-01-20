#ifndef SPRITE_TYPE_H_
#define SPRITE_TYPE_H_

namespace mengde {
namespace gui {
namespace uifw {

// Sprite Type

enum SpriteType {
#define MACRO_SPRITE_TYPE(ename, bname, dir, ofs, snum, bsize) kSprite##ename,
#include "sprite_type.inc.h"
  kSpriteCount
};

enum SpriteEffectType {
  kEffectNone,
  kEffectFade,
  kEffectShade,
  kEffectBright,
  kEffectTint
};

struct SpriteEffect {
  SpriteEffectType effect;
  int              progress;
};

extern const char* kSpriteToString[];
extern const char* kSpriteBitmapName[];
extern const bool  kSpriteHasDir[];
extern const int   kSpriteOffset[];
extern const int   kSpriteNumSprites[];
extern const int   kSpriteBitmapSize[];

} // namespace uifw
} // namespace gui
} // namespace mengde

#endif // SPRITE_TYPE_H_
