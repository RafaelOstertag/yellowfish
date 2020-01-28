#ifndef __SINE_HH
#define __SINE_HH

#include "../sdl/renderable.hh"

namespace screens {
class Sine : public sdl::Renderable {
  public:
    Sine(Sint32 x, Sint32 y, Sint32 width, Sint32 height,
         bool clearScreen = true);
    virtual ~Sine() {}

    virtual void render(const sdl::Renderer& renderer);

  private:
    SDL_Rect rect;
    int calls;
    uint8_t xOffset;
    bool clearScreen;
};
} // namespace screens
#endif
