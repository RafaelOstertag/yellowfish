#include "image.hh"

#include <SDL_image.h>

#include <sstream>
#include <stdexcept>

using namespace screens;

namespace {
constexpr int doNotFree{0};
}

Image::Image(const sdl::MemoryRWOps& memory)
    : surface{nullptr}, texture{nullptr} {
    surface = IMG_Load_RW(memory.getRWOps(), doNotFree);
    if (surface == nullptr) {
        std::ostringstream errorMessage;
        errorMessage << "Error loading bitmap from memory: " << SDL_GetError();

        throw std::invalid_argument(errorMessage.str());
    }
}

Image::Image() : surface{nullptr}, texture{nullptr} {}

Image::~Image() { cleanup(); }

Image::Image(Image&& o) : surface{o.surface}, texture{o.texture} {
    o.surface = nullptr;
    o.texture = nullptr;
}

Image& Image::operator=(Image&& o) {
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
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == nullptr) {
            std::ostringstream errorMessage;
            errorMessage << "Cannot convert surface to texture: "
                         << SDL_GetError();

            throw std::runtime_error(errorMessage.str());
        }

        SDL_FreeSurface(surface);
        surface = nullptr;
    }

    SDL_RenderCopy(renderer, texture, NULL, NULL);
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
