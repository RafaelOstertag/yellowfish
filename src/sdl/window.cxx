#include "window.hh"

#include <iostream>

#include "sdl_error.hh"

using namespace sdl;

Window::Window(const std::string& name, int width, int height,
               const Color& bgColor, bool fullscreen)
    :  bgColor{bgColor} {
    Uint32 windowFlags = SDL_WINDOW_SHOWN;
    if (fullscreen) {
        windowFlags |= SDL_WINDOW_FULLSCREEN;
    }

    sdlWindow =
        SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, width, height, windowFlags);
    if (sdlWindow == nullptr) {
        std::string errorMsg{"Cannot initialize window: "};
        throw SDLError(errorMsg + SDL_GetError());
    }

    auto sdlRenderer =
        SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
    if (sdlRenderer == nullptr) {
        std::string errorMsg{"Cannot initialize renderer: "};
        throw SDLError(errorMsg + SDL_GetError());
    }
    renderer = Renderer{sdlRenderer};
}

Window::Window(Window&& o) noexcept
    : sdlWindow{o.sdlWindow}, bgColor{o.bgColor} {
    o.sdlWindow = nullptr;
}

Window::~Window() {
    if (sdlWindow) SDL_DestroyWindow(sdlWindow);
}

void Window::clear() {
    SDL_SetRenderDrawColor(renderer.get(), bgColor.red(), bgColor.green(),
                           bgColor.blue(), bgColor.alpha());
    SDL_RenderClear(renderer.get());
}

void Window::render(Renderable& renderable) const {
    renderable.render(renderer);
}

void Window::update() const { SDL_RenderPresent(renderer.get()); }
