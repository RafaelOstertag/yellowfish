#ifndef __CLOCK_HH
#define __CLOCK_HH

#include "../sdl/color.hh"
#include "../sdl/font.hh"
#include "../sdl/renderable.hh"
#include <string>

namespace screens {
class Clock : public sdl::Renderable {
  public:
    Clock(const std::string& fontpath, int size, const sdl::Color& fontColor, bool showSeconds = false);
    virtual ~Clock() {}

    virtual void render(const sdl::Renderer& renderer);

  private:
    sdl::FontPtr font;
    sdl::Color color;
    bool showSeconds;

    SDL_Texture* textToTexture(const std::string& text,
                               const sdl::Renderer& renderer);
};
} // namespace screens

#endif
