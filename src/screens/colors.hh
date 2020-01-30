#ifndef __COLORS_HH
#define __COLORS_HH

#include "../sdl/color.hh"
#include "../sdl/renderable.hh"

namespace screens {
class Colors : public sdl::Renderable {
   public:
    Colors(Sint32 x, Sint32 y, Sint32 width, Sint32 height);
    virtual ~Colors() {}

    virtual void render(const sdl::Renderer& renderer);

   private:
    SDL_Rect rect;
    sdl::Color color;
    bool redUp;
    bool greenUp;
    bool blueUp;

    void setNextColor();
    Uint8 nextRed(Uint8 red);
    Uint8 nextGreen(Uint8 green);
    Uint8 nextBlue(Uint8 blue);
};
}  // namespace screens

#endif
