#include "colors.hh"

using namespace screens;

Colors::Colors(Sint32 x, Sint32 y, Sint32 width, Sint32 height)
    : rect{x, y, width, height}, redUp{true}, greenUp{true}, blueUp{true} {}

void Colors::render(const sdl::Renderer& renderer) {
    SDL_SetRenderDrawColor(renderer, color.red(), color.green(), color.blue(),
                           color.alpha());
    SDL_RenderFillRect(renderer, &rect);
    setNextColor();
}

void Colors::setNextColor() {
    auto red = color.red();
    auto green = color.green();
    auto blue = nextBlue(color.blue());

    if (green == 0x0 || green == 0xff) {
        red = nextRed(red);
    }

    if (blue == 0x0 || blue == 0xff) {
        green = nextGreen(green);
    }

    color = sdl::Color{red, green, blue, color.alpha()};
}

Uint8 Colors::nextRed(Uint8 red) {
    if (redUp) {
        red++;
    } else {
        red--;
    }

    switch (red) {
    case 0x0:
        redUp = true;
        break;
    case 0xff:
        redUp = false;
        break;
    }
    return red;
}

Uint8 Colors::nextGreen(Uint8 green) {
    if (greenUp) {
        green++;
    } else {
        green--;
    }

    switch (green) {
    case 0x0:
        greenUp = true;
        break;
    case 0xff:
        greenUp = false;
        break;
    }
    return green;
}

Uint8 Colors::nextBlue(Uint8 blue) {
    if (blueUp) {
        blue++;
    } else {
        blue--;
    }

    switch (blue) {
    case 0x0:
        blueUp = true;
        break;
    case 0xff:
        blueUp = false;
        break;
    }
    return blue;
}
