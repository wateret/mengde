#include "renderer.h"
#include "misc.h"
#include "texture.h"

Renderer::Renderer(Window* window) {
  handle_ = SDL_CreateRenderer(window->AsRawWindow(),
                               -1,
#ifdef DEBUG
                               SDL_RENDERER_ACCELERATED);
#else
                               SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
#endif
  if (handle_ == NULL)
    throw Misc::GetErrorMessage() + " - SDL_CreateRenderer";
  SDL_SetRenderDrawColor(handle_, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_SetRenderDrawBlendMode(handle_, SDL_BLENDMODE_BLEND);

  pixel_format_ = SDL_GetWindowPixelFormat(window->AsRawWindow());
  pixel_format_obj_ = SDL_AllocFormat(pixel_format_);
}

Renderer::~Renderer() {
	SDL_DestroyRenderer(handle_);
  SDL_FreeFormat(pixel_format_obj_);
}

void Renderer::DrawRect(const Rect* rect, const int border) {
  SDL_Rect raw_rect = *rect->GetRawRectPtr();
  for (int b = 0; b < border; b++) {
    // border goes to inside
    SDL_RenderDrawRect(handle_, &raw_rect);
    raw_rect.x++;
    raw_rect.y++;
    raw_rect.w -= 2;
    raw_rect.h -= 2;
  }
}

void Renderer::FillRect(const Rect* rect) {
  const SDL_Rect* raw_rect = rect->GetRawRectPtr();
  SDL_RenderFillRect(handle_, raw_rect);
}

void Renderer::Clear() {
  SDL_RenderClear(handle_);
}

void Renderer::SetDrawColor(Color color) {
  SDL_SetRenderDrawColor(handle_, color.r, color.g, color.b, color.a);
}

void Renderer::Present() {
  SDL_RenderPresent(handle_);
}

void Renderer::CopyTexture(Texture* texture,
                           Rect* srcRect,
                           Rect* dstRect,
                           bool flip_hor) {
  const SDL_Rect* src_rect_raw = (srcRect == NULL) ?
                                 NULL : srcRect->GetRawRectPtr();
  const SDL_Rect* dst_rect_raw = (dstRect == NULL) ?
                                 NULL : dstRect->GetRawRectPtr();

  SDL_Texture* raw_texture = texture->AsRawTexture();

  if (texture == NULL) {
    throw Misc::GetErrorMessage() +
          " - SDL_CreateTextureFromSurface in Renderer::copyTexture";
  }
  if (flip_hor) {
    if (SDL_RenderCopyEx(handle_,
                         raw_texture,
                         src_rect_raw,
                         dst_rect_raw,
                         0.0,
                         NULL,
                         SDL_FLIP_HORIZONTAL) != 0) {
      throw Misc::GetErrorMessage() +
            " - SDL_RenderCopyEx in Renderer::CopyTexture";
    }
  } else {
    if (SDL_RenderCopy(handle_,
                       raw_texture,
                       src_rect_raw,
                       dst_rect_raw) != 0) {
      throw Misc::GetErrorMessage() +
            " - SDL_RenderCopy in Renderer::CopyTexture";
    }
  }
}

SDL_Renderer* Renderer::GetRawRenderer() {
  return handle_;
}

uint32_t Renderer::GetPixelFormat() {
  return pixel_format_;
}

void Renderer::SetViewport(const Rect* r) {
  SDL_RenderSetViewport(handle_, r->GetRawRectPtr());
}

SDL_PixelFormat* Renderer::GetRawPixelFormatObj() {
  return pixel_format_obj_;
}
