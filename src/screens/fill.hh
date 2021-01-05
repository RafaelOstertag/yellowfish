#ifndef __FILL_HH
#define __FILL_HH

#include "../sdl/color.hh"
#include "../sdl/renderable.hh"

namespace screens {
class Fill : public sdl::Renderable {
   public:
    explicit Fill(const sdl::Color& fillColor) : fillColor{fillColor} {}

    void render(const sdl::Renderer& renderer) override {
        SDL_SetRenderDrawColor(renderer.get(), fillColor.red(), fillColor.green(),
                               fillColor.blue(), fillColor.alpha());

        SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);

        SDL_RenderFillRect(renderer.get(), nullptr);
        SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_NONE);
    }

   protected:
    void setFillColor(const sdl::Color& fc) { fillColor = fc; }

   private:
    sdl::Color fillColor;
};
}  // namespace screens

#endif
