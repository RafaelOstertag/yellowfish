#include "image.hh"

#include <SDL_image.h>

#include <sstream>
#include <stdexcept>

#include "screen_error.hh"

using namespace screens;

namespace {
constexpr int doNotFree{0};
}

Image::Image(const sdl::MemoryRWOps& memory) {
    surface = IMG_Load_RW(memory.getRWOps(), doNotFree);
    if (surface == nullptr) {
        std::ostringstream errorMessage;
        errorMessage << "Error loading bitmap from memory: " << SDL_GetError();

        throw ScreenError(errorMessage.str());
    }
}

Image::~Image() { cleanup(); }

Image::Image(Image&& o) noexcept : surface{o.surface}, texture{o.texture} {
    o.surface = nullptr;
    o.texture = nullptr;
}

Image& Image::operator=(Image&& o) noexcept {
    if (&o == this) {
        return *this;
    }

    cleanup();
    surface = o.surface;
    o.surface = nullptr;

    texture = o.texture;
    o.texture = nullptr;

    return *this;
}

void Image::render(const sdl::Renderer& renderer) {
    if (texture == nullptr) {
        texture = SDL_CreateTextureFromSurface(renderer.get(), surface);
        if (texture == nullptr) {
            std::ostringstream errorMessage;
            errorMessage << "Cannot convert surface to texture: "
                         << SDL_GetError();

            throw ScreenError(errorMessage.str());
        }

        SDL_FreeSurface(surface);
        surface = nullptr;
    }

    SDL_RenderCopy(renderer.get(), texture, nullptr, nullptr);
}

void Image::cleanup() {
    if (surface != nullptr) {
        SDL_FreeSurface(surface);
        surface = nullptr;
    }
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}
