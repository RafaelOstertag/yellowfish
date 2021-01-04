#ifndef __IMAGE_HH
#define __IMAGE_HH

#include "../sdl/memoryrwops.hh"
#include "../sdl/renderable.hh"

namespace screens {
class Image final : public sdl::Renderable {
   public:
    Image() = default;
    explicit Image(const sdl::MemoryRWOps& memory);

    ~Image() override;

    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;

    Image(Image&& o) noexcept;
    Image& operator=(Image&& o) noexcept;

    void render(const sdl::Renderer& renderer) override;

    constexpr bool isEmpty() const {
        return surface == nullptr && texture == nullptr;
    }

   private:
    SDL_Surface* surface{nullptr};
    SDL_Texture* texture{nullptr};

    void cleanup();
};
}  // namespace screens

#endif
