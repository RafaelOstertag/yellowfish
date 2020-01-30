#include "sine.hh"

#include <cmath>

using namespace screens;

Sine::Sine(Sint32 x, Sint32 y, Sint32 width, Sint32 height, bool clearScreen)
    : rect{x, y, width, height},
      calls{0},
      xOffset{0},
      clearScreen{clearScreen} {}

void Sine::render(const sdl::Renderer& renderer) {
    int baseLine = rect.y + rect.h / 2;

    if (clearScreen) {
        SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0x0);
        SDL_RenderClear(renderer);
    }

    for (int currentX = rect.x; currentX < rect.w; currentX++) {
        SDL_SetRenderDrawColor(renderer, 0x80 + currentX, 0xf + currentX,
                               0x1f + currentX, 0x0);
        auto offset = 150.0 * std::sin(currentX * 0.009 + xOffset);
        SDL_RenderDrawPoint(renderer, currentX, baseLine + offset);
    }

    if (calls % 13 == 0) {
        xOffset++;
        calls = 0;
    }

    calls++;
}
