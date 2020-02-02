#ifndef __CLOCK_HH
#define __CLOCK_HH

#include <string>

#include "../sdl/color.hh"
#include "../sdl/font.hh"
#include "../sdl/renderable.hh"

namespace screens {
class Clock : public sdl::Renderable {
   public:
    enum Alignment { TOP_RIGHT, TOP_LEFT, BOTTOM_RIGHT, BOTTOM_LEFT, CENTER };

    Clock(const std::string& fontpath, int size, const sdl::Color& fontColor,
          Alignment alignment = CENTER, bool showSeconds = false);
    virtual ~Clock() {}

    Clock(const Clock&) = delete;
    Clock(Clock&&) = delete;

    virtual void render(const sdl::Renderer& renderer);

   private:
    sdl::FontPtr font;
    sdl::Color color;
    Alignment alignment;
    bool showSeconds;

    SDL_Texture* textToTexture(const std::string& text,
                               const sdl::Renderer& renderer);
};
}  // namespace screens

#endif
