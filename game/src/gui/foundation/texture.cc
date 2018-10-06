#include "texture.h"

#include "misc.h"
#include "renderer.h"
#include "util/game_env.h"

namespace mengde {
namespace gui {
namespace foundation {

Texture::Texture(Renderer* renderer, const std::string& path) { InitBitmap(renderer, path, false, Color(0, 0, 0)); }

Texture::Texture(Renderer* renderer, const std::string& path, Color colorKey) {
  InitBitmap(renderer, path, true, colorKey);
}

Texture::Texture(Renderer* renderer, SDL_Surface* surface) {
  surface_ = surface;
  CreateFromSurface(renderer, surface);
}

Texture::Texture(SDL_Texture* texture, int width, int height)
    : surface_(nullptr), texture_(texture), width_(width), height_(height) {}

Texture::Texture(Renderer* renderer, const std::string& text, TTF_Font* font, Color color, uint32_t wrap,
                 uint32_t style_flags)
    : surface_(nullptr) {
  // Create a text texture
  // We do not keep surface_ for text textures
  int raw_style = style_flags;

  TTF_SetFontStyle(font, raw_style);
  SDL_Surface* surface;
  if (wrap == 0) {
    surface = TTF_RenderUTF8_Blended(font, text.c_str(), color.ToRawColor());
  } else {
    surface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color.ToRawColor(), wrap);
  }
  TTF_SetFontStyle(font, TTF_STYLE_NORMAL);

  if (surface == NULL) {
    string msg = Misc::GetErrorMessage();
    LOG_FATAL("TTF_RenderText_Blended Failed : %s", msg.c_str());
  }
  CreateFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
}

void Texture::InitBitmap(Renderer* renderer, const std::string& path, bool use_key, Color color) {
  surface_ = SDL_LoadBMP(path.c_str());
  if (surface_ == nullptr) {
    string msg = Misc::GetErrorMessage();
    LOG_FATAL("Unable to load image! : %s", msg.c_str());
    texture_ = nullptr;
    return;
  }

  if (use_key) {
    unsigned int key = SDL_MapRGB(surface_->format, color.r, color.g, color.b);
    SDL_SetColorKey(surface_, SDL_TRUE, key);
  }

  CreateFromSurface(renderer, surface_);
}

int Texture::w() { return width_; }

int Texture::h() { return height_; }

Vec2D Texture::size() { return {width_, height_}; }

void Texture::SetAlpha(uint8_t alpha) { SDL_SetTextureAlphaMod(texture_, alpha); }

void Texture::SetShade(uint8_t b) { SDL_SetTextureColorMod(texture_, b, b, b); }

void Texture::CreateFromSurface(Renderer* renderer, SDL_Surface* surface) {
  width_ = surface->w;
  height_ = surface->h;
  texture_ = SDL_CreateTextureFromSurface(renderer->GetRawRenderer(), surface);
}

void Texture::FreeIfLoaded() {
  // We do not free Texture here since they are
  // TODO what if we erase some textures in texture manager?
  if (surface_ != nullptr) {
    SDL_FreeSurface(surface_);
  }
}

Texture::~Texture() { FreeIfLoaded(); }

SDL_Texture* Texture::AsRawTexture() { return texture_; }

Texture* Texture::NewWhitenedTexture(Renderer* renderer) {
  const uint32_t pixel_format = renderer->GetPixelFormat();
  ASSERT(surface_ != nullptr);

  SDL_Surface* fsurface = SDL_ConvertSurfaceFormat(surface_, pixel_format, 0);
  SDL_PixelFormat* mapping_format = renderer->GetRawPixelFormatObj();
  uint32_t transparent = SDL_MapRGBA(mapping_format, 0x00, 0x00, 0x00, 0x00);
  uint32_t white = SDL_MapRGBA(mapping_format, 0xFF, 0xFF, 0xFF, 0xFF);
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_; j++) {
      const uint32_t offset = i * (fsurface->pitch / 4) + j;
      uint32_t* pixel = ((uint32_t*)fsurface->pixels) + offset;
      uint8_t r, g, b, a;
      SDL_GetRGBA(*pixel, mapping_format, &r, &g, &b, &a);
      if (a == 0) {
        *pixel = transparent;
      } else {
        *pixel = white;
      }
    }
  }
  SDL_Texture* raw_texture = SDL_CreateTextureFromSurface(renderer->GetRawRenderer(), fsurface);
  SDL_SetTextureBlendMode(raw_texture, SDL_BLENDMODE_BLEND);
  SDL_FreeSurface(fsurface);

  return new Texture{raw_texture, width_, height_};
}

}  // namespace foundation
}  // namespace gui
}  // namespace mengde
