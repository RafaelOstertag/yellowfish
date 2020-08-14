#ifndef __COLOR_HH
#define __COLOR_HH

#include <SDL.h>

namespace sdl {
class Color {
   public:
    Color(unsigned char red = 0, unsigned char green = 0,
          unsigned char blue = 0, unsigned char alpha = 0xff)
        : sdlColor{red, green, blue, alpha} {}

    constexpr unsigned char red() const { return sdlColor.r; }
    constexpr unsigned char green() const { return sdlColor.g; }
    constexpr unsigned char blue() const { return sdlColor.b; }
    constexpr unsigned char alpha() const { return sdlColor.a; }

    constexpr operator const SDL_Color*() const { return &sdlColor; }
    constexpr operator SDL_Color() const { return sdlColor; }

   private:
    SDL_Color sdlColor;
};

const Color WHITE{0xff, 0xff, 0xff, 0x0};
const Color BLACK{0x0, 0x0, 0x0, 0x0};
}  // namespace sdl
#endif
