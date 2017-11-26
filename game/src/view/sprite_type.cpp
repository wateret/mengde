#include "sprite_type.h"

#include "common.h"

const char* kSpriteToString[] = {
#define MACRO_SPRITE_TYPE(ename, bname, dir, ofs, snum, bsize) #ename,
#include "sprite_type.inc.h"
  NULL
};

const char* kSpriteBitmapName[] = {
#define MACRO_SPRITE_TYPE(ename, bname, dir, ofs, snum, bsize) bname,
#include "sprite_type.inc.h"
  NULL
};

const bool kSpriteHasDir[] = {
#define MACRO_SPRITE_TYPE(ename, bname, dir, ofs, snum, bsize) dir,
#include "sprite_type.inc.h"
  0
};

const int kSpriteOffset[] = {
#define MACRO_SPRITE_TYPE(ename, bname, dir, ofs, snum, bsize) ofs,
#include "sprite_type.inc.h"
  0
};

const int kSpriteNumSprites[] = {
#define MACRO_SPRITE_TYPE(ename, bname, dir, ofs, snum, bsize) snum,
#include "sprite_type.inc.h"
  0
};

const int kSpriteBitmapSize[] = {
#define MACRO_SPRITE_TYPE(ename, bname, dir, ofs, snum, bsize) bsize,
#include "sprite_type.inc.h"
  0
};

