#ifndef __IMAGE_HH
#define __IMAGE_HH

#include "../sdl/memoryrwops.hh"
#include "../sdl/renderable.hh"

namespace screens {
class Image : public sdl::Renderable {
  public:
    Image();
    Image(const sdl::MemoryRWOps& memory);

    virtual ~Image();

    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;

    Image(Image&& o);
    Image& operator=(Image&& o);

    virtual void render(const sdl::Renderer& renderer);

    constexpr bool isEmpty() {
        return surface == nullptr && texture == nullptr;
    }

  private:
    SDL_Surface* surface;
    SDL_Texture* texture;

    void cleanup();
};
} // namespace screens

#endif
