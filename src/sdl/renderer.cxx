#include "renderer.hh"

using namespace sdl;

Renderer::Renderer(SDL_Renderer* renderer) : sdlRenderer{renderer} {}

Renderer::~Renderer() { destroyRenderer(); }

Renderer::Renderer(Renderer&& o) noexcept : sdlRenderer{o.sdlRenderer} {
    o.sdlRenderer = nullptr;
}

Renderer& Renderer::operator=(Renderer&& o) noexcept {
    destroyRenderer();

    sdlRenderer = o.sdlRenderer;
    o.sdlRenderer = nullptr;

    return *this;
}

void Renderer::destroyRenderer() {
    if (sdlRenderer != nullptr) {
        SDL_DestroyRenderer(sdlRenderer);
        sdlRenderer = nullptr;
    }
}
