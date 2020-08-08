#ifndef __FILL_HH
#define __FILL_HH

#include "../sdl/color.hh"
#include "../sdl/renderable.hh"

namespace screens {
class Fill : public sdl::Renderable {
   public:
    Fill(const sdl::Color& fillColor) : fillColor{fillColor} {}
    virtual ~Fill() = default;

    virtual void render(const sdl::Renderer& renderer) {
        SDL_SetRenderDrawColor(renderer, fillColor.red(), fillColor.green(),
                               fillColor.blue(), fillColor.alpha());

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        SDL_RenderFillRect(renderer, NULL);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }

   protected:
    void setFillColor(const sdl::Color& fc) { fillColor = fc; }

   private:
    sdl::Color fillColor;
};
}  // namespace screens

#endif
