#ifndef __RENDERABLE_HH
#define __RENDERABLE_HH

#include "renderer.hh"

namespace sdl {
class Renderable {
   public:
    virtual void render(const Renderer& renderer) = 0;
};
}  // namespace sdl

#endif
