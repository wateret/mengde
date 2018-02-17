#include "drawer.h"

#include "gui/app/app.h"  // XXX Remove this depenency
#include "gui/foundation/rect.h"
#include "gui/foundation/renderer.h"
#include "gui/foundation/texture.h"
#include "gui/foundation/texture_manager.h"
#include "gui/foundation/window.h"

namespace mengde {
namespace gui {
namespace uifw {

Drawer::Drawer(Window* window, const string& scenario_path, const string& font_path)
    : window_size_(window->GetSize()), offset_(0, 0), viewports_() {
  renderer_        = new Renderer(window);
  texture_manager_ = new TextureManager(renderer_, scenario_path, font_path);
  viewports_.push(Viewport(Rect(0, 0, window_size_.x, window_size_.y), {0, 0}));
}

Drawer::~Drawer() {
  delete texture_manager_;
  delete renderer_;
}

void Drawer::Start() { renderer_->Clear(); }

void Drawer::End() { renderer_->Present(); }

void Drawer::SetDrawColor(Color c) { renderer_->SetDrawColor(c); }

void Drawer::CopyTexture(Texture* texture, Rect* rect_src, Rect* rect_dst, bool flip_hor) {
  //  ASSERT(rect_src != NULL);
  ASSERT(rect_dst != NULL);
  Rect rr = *rect_dst;
  rr.Move(-offset_);
  rr.Move(viewports_.top().neg_coords);
  renderer_->CopyTexture(texture, rect_src, &rr, flip_hor);
}

void Drawer::CopyTextureToCell(Texture* texture, Rect* rect_src, Vec2D pos) {
  Vec2D dst    = pos * app::App::kBlockSize;
  Vec2D size   = rect_src->GetSize();
  Vec2D adjust = (size - app::App::kBlockSize) / 2;
  Rect  rect_dst(dst - adjust, size);
  CopyTexture(texture, rect_src, &rect_dst);
}

void Drawer::CopyTextureBackground(Texture* texture, Rect* rect_src, Rect* rect_dst) {
  Rect rr = *rect_src;
  rr.Move(offset_);
  rr.Move(viewports_.top().neg_coords);
  renderer_->CopyTexture(texture, &rr, rect_dst);
}

void Drawer::CopySprite(const std::string& path, SpriteType type, Direction dir, int sprite_no, SpriteEffect effect,
                        Vec2D dst_coords, Vec2D offset) {
  std::string fullpath    = "model/" + path + "/" + kSpriteBitmapName[type] + ".bmp";
  Texture*    bitmap      = texture_manager_->FetchTexture(fullpath);
  const int   bitmap_size = kSpriteBitmapSize[type];

  int  sprite_offset = 0;
  bool flip_hor      = false;
  if (kSpriteHasDir[type]) {
    switch (dir) {
      case kDirDown:
        sprite_offset = 0;
        break;
      case kDirUp:
        sprite_offset = 1;
        break;
      case kDirLeft:
        sprite_offset = 2;
        break;
      case kDirRight:
        sprite_offset = 2;
        flip_hor      = true;
        break;
      default:
        ASSERT(dir == kDirNone);
        sprite_offset = 0;
        break;
    }
  }
  sprite_offset *= kSpriteNumSprites[type];
  int sprite_index = kSpriteOffset[type] + sprite_offset + sprite_no;

  // FIXME Can we use CopyTextureToCell()? instead of calculating coords here
  Vec2D dst      = dst_coords * app::App::kBlockSize + offset;
  int   adjust   = (bitmap_size - app::App::kBlockSize) / 2;
  Rect  src_rect = {0, sprite_index, 1, 1};
  src_rect.Magnify(bitmap_size);
  Rect dst_rect = {dst.x - adjust, dst.y - adjust, bitmap_size, bitmap_size};

  // TODO better way for this? Instead of the before-after sequence
  switch (effect.effect) {
    case kEffectFade:
      bitmap->SetAlpha(255 - effect.progress);
      break;
    case kEffectShade:
      bitmap->SetShade(255 - effect.progress);
      break;
    case kEffectBright: {
      bitmap->SetAlpha(255 - effect.progress);
      Texture* whitened_bitmap = texture_manager_->FetchWhitenedTexture(fullpath);
      CopyTexture(whitened_bitmap, &src_rect, &dst_rect, flip_hor);
      break;
    }
    default:
      break;
  }

  CopyTexture(bitmap, &src_rect, &dst_rect, flip_hor);

  switch (effect.effect) {
    case kEffectFade:
      bitmap->SetAlpha(255);
      break;
    case kEffectShade:
      bitmap->SetShade(255);
      break;
    case kEffectBright: {
      bitmap->SetAlpha(255);
      break;
    }
    default:
      break;
  }
}

void Drawer::DrawRect(const Rect* r, const int border) {
  Rect rr = *r;
  rr.Move(-offset_);
  rr.Move(viewports_.top().neg_coords);
  renderer_->DrawRect(&rr, border);
}

void Drawer::FillRect(const Rect* r) {
  Rect rr = *r;
  rr.Move(-offset_);
  rr.Move(viewports_.top().neg_coords);
  renderer_->FillRect(&rr);
}

void Drawer::DrawText(const std::string& text, int size, Color color, Vec2D pos) {
  if (text == "") return;
  Texture* texture = texture_manager_->FetchTextTexture(text, size, color);
  Rect     rect(pos, texture->GetSize());
  CopyTexture(texture, NULL, &rect);
}

void Drawer::DrawText(const std::string& text, int size, Color color, const Rect* frame, LayoutHelper::Align align) {
  if (text == "") return;
  Texture* texture = texture_manager_->FetchTextTexture(text, size, color, frame->GetW());
  Rect     rect    = LayoutHelper::CalcPosition(frame, texture->GetSize(), align);
  CopyTexture(texture, NULL, &rect);
}

void Drawer::BorderCell(Vec2D c, const int b) {
  Rect rect(c.x, c.y, 1, 1);
  rect.Magnify(app::App::kBlockSize);
  DrawRect(&rect, b);
}

void Drawer::FillCell(Vec2D c) {
  Rect rect(c.x, c.y, 1, 1);
  rect.Magnify(app::App::kBlockSize);
  FillRect(&rect);
}

void Drawer::SetViewport(const Rect* r) {
  Viewport top_vp = viewports_.top();

  Rect& rect = top_vp.rect;
  Rect  rr(rect.GetPos() + r->GetPos() + top_vp.neg_coords, r->GetSize());
  ASSERT(rr.GetW() >= 0 && rr.GetH() >= 0);

  Vec2D neg_coords;
  neg_coords.x = std::min(0, rr.GetX() - rect.GetX());
  neg_coords.y = std::min(0, rr.GetY() - rect.GetY());
  rr.Move(-neg_coords);
  //  rr -= neg_coords;
  int rgt = std::min(0, rect.GetRight() - rr.GetRight());
  int bot = std::min(0, rect.GetBottom() - rr.GetBottom());
  rr += {rgt, bot};

  Viewport new_vp(rr, neg_coords);
  //  LOG_INFO("Viewport push (%d %d %d %d) (%d %d)", rr.GetX(), rr.GetY(), rr.GetW(), rr.GetH(), neg_coords.x,
  //  neg_coords.y);
  renderer_->SetViewport(&new_vp.rect);
  viewports_.push(new_vp);
}

void Drawer::ResetViewport() {
  viewports_.pop();
  Viewport vp = viewports_.top();
  //  LOG_INFO("Viewport pop  (%d %d %d %d)", vp.rect.GetX(), vp.rect.GetY(), vp.rect.GetW(), vp.rect.GetH());
  renderer_->SetViewport(&vp.rect);
}

}  // namespace uifw
}  // namespace gui
}  // namespace mengde
