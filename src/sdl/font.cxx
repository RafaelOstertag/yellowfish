#include "font.hh"

#include <SDL.h>

#include <stdexcept>

using namespace sdl;

Font::Font(const std::string& filepath, int size)
    : ttfFont{TTF_OpenFont(filepath.c_str(), size), TTF_CloseFont} {
    if (ttfFont == nullptr) {
        std::string errmsg{"Cannot load font: "};
        throw std::invalid_argument(errmsg + SDL_GetError());
    }
}

Font::~Font() {}

Font::Font(const Font& o) { ttfFont = o.ttfFont; }

Font& Font::operator=(const Font& o) {
    if (this == &o) {
        return *this;
    }

    ttfFont = o.ttfFont;

    return *this;
}

Font::Font(Font&& o) : ttfFont{std::move(o.ttfFont)} {}

Font& Font::operator=(Font&& o) {
    ttfFont = std::move(o.ttfFont);
    o.ttfFont = nullptr;

    return *this;
}
