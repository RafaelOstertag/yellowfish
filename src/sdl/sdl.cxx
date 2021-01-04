#include "sdl.hh"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <sstream>

#include "sdl_error.hh"

using namespace sdl;

bool SDL::initialized = false;

namespace {
void getDisplayBounds(int displayIndex, SDL_Rect* r) {
    if (SDL_GetDisplayBounds(displayIndex, r) != 0) {
        std::stringstream errorMsg;
        errorMsg << "Unable to get display bounds: " << SDL_GetError();
        throw SDLError(errorMsg.str());
    }
}
}  // namespace

void SDL::initialize() {
    if (initialized) {
#ifndef NDEBUG
        std::cerr << "Already initialized\n";
#endif
        return;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Cannot initialize SDL: " << SDL_GetError() << "\n";
        initialized = false;
        return;
    }

    if (TTF_Init() == -1) {
        SDL_Quit();

        std::cerr << "Cannot initialize SDL TTF: " << TTF_GetError() << "\n";
        initialized = false;
        return;
    }

    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    auto result = IMG_Init(imgFlags);
    if ((result & imgFlags) != imgFlags) {
        TTF_Quit();
        SDL_Quit();

        std::cerr << "Cannot initialize JPEG and PNG support: "
                  << IMG_GetError() << '\n';
        initialized = false;
        return;
    }

    initialized = true;
}

void SDL::shutdown() {
    if (!initialized) {
#ifndef NDEBUG
        std::cerr << "Not initialized, won't shutdown\n";
#endif
        return;
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    initialized = false;
}

int SDL::displayWidth() {
    if (!initialized) {
        throw SDLError("SDL not initialized");
    }

    SDL_Rect r;
    getDisplayBounds(0, &r);
    return r.w;
}

int SDL::displayHeight() {
    if (!initialized) {
        throw SDLError("SDL not initialized");
    }

    SDL_Rect r;
    getDisplayBounds(0, &r);
    return r.h;
}
