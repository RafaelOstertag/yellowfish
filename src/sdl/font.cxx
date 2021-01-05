#include "font.hh"

#include <SDL.h>

#include "sdl_error.hh"

using namespace sdl;

Font::Font(const std::string& filepath, int size)
    : ttfFont{TTF_OpenFont(filepath.c_str(), size), TTF_CloseFont} {
    if (ttfFont == nullptr) {
        std::string errmsg{"Cannot load font: "};
        throw SDLError(errmsg + SDL_GetError());
    }
}
