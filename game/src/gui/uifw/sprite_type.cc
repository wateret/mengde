#include "sprite_type.h"

namespace mengde {
namespace gui {
namespace uifw {

const char* kSpriteToString[] = {
#define MACRO_SPRITE_TYPE(ename, bname, dir, ofs, snum, bsize) #ename,
#include "sprite_type.h.inc"
    nullptr};

const char* kSpriteBitmapName[] = {
#define MACRO_SPRITE_TYPE(ename, bname, dir, ofs, snum, bsize) bname,
#include "sprite_type.h.inc"
    nullptr};

const bool kSpriteHasDir[] = {
#define MACRO_SPRITE_TYPE(ename, bname, dir, ofs, snum, bsize) dir,
#include "sprite_type.h.inc"
    0};

const int kSpriteOffset[] = {
#define MACRO_SPRITE_TYPE(ename, bname, dir, ofs, snum, bsize) ofs,
#include "sprite_type.h.inc"
    0};

const int kSpriteNumSprites[] = {
#define MACRO_SPRITE_TYPE(ename, bname, dir, ofs, snum, bsize) snum,
#include "sprite_type.h.inc"
    0};

const int kSpriteBitmapSize[] = {
#define MACRO_SPRITE_TYPE(ename, bname, dir, ofs, snum, bsize) bsize,
#include "sprite_type.h.inc"
    0};

}  // namespace uifw
}  // namespace gui
}  // namespace mengde
