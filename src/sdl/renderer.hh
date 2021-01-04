#ifndef __RENDERER_HH
#define __RENDERER_HH

#include <SDL.h>

namespace sdl {
class Renderer final {
   public:
    explicit Renderer(SDL_Renderer* renderer = nullptr);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    Renderer(Renderer&&) noexcept;
    Renderer& operator=(Renderer&& o) noexcept;

    SDL_Renderer* get() const { return sdlRenderer; }

   private:
    SDL_Renderer* sdlRenderer;

    void destroyRenderer();
};
}  // namespace sdl

#endif
