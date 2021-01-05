#ifndef __TEXTRENDERER_HH
#define __TEXTRENDERER_HH

#include <stdexcept>
#include <string>

#include "../sdl/color.hh"
#include "../sdl/font.hh"
#include "../sdl/renderer.hh"

namespace utils {

class RenderError final : public std::runtime_error {
   public:
    using std::runtime_error::runtime_error;
};

SDL_Texture* textToTexture(const std::string& text, const sdl::Font& font,
                           const sdl::Color& color,
                           const sdl::Renderer& renderer);
}  // namespace utils

#endif
