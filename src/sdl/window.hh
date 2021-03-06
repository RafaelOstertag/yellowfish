#ifndef __WINDOW_HH
#define __WINDOW_HH

#include <SDL.h>

#include <memory>
#include <string>

#include "color.hh"
#include "renderable.hh"
#include "renderer.hh"

namespace sdl {
class Window final {
   public:
    Window(const std::string& name, int width, int heigh, const Color& bgColor,
           bool fullscreen = false);
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    Window(Window&& o) noexcept;
    Window& operator=(Window&&) = delete;

    void clear();
    void render(Renderable& renderable) const;
    void update() const;
    constexpr const Renderer& getRenderer() const { return renderer; }

   private:
    SDL_Window* sdlWindow{nullptr};
    Renderer renderer{};
    const Color& bgColor;
};

using WindowPtr = std::shared_ptr<Window>;
}  // namespace sdl
#endif
