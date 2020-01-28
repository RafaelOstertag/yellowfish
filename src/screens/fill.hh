#ifndef __FILL_HH
#define __FILL_HH

#include "../sdl/renderable.hh"

namespace screens {
class Fill : public sdl::Renderable {
  public:
    Fill(const sdl::Color& fillColor) : fillColor{fillColor} {}
    virtual ~Fill() {}

    virtual void render(const sdl::Renderer& renderer) {
        SDL_SetRenderDrawColor(renderer, fillColor.red(), fillColor.green(),
                               fillColor.blue(), fillColor.alpha());

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        SDL_RenderFillRect(renderer, NULL);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }

  private:
    sdl::Color fillColor;
};
} // namespace screens

#endif
