#ifndef __FONT_HH
#define __FONT_HH

#include <SDL_ttf.h>

#include <memory>
#include <string>

namespace sdl {

class Font final {
   public:
    Font(const std::string& filepath, int size);

    TTF_Font* get() const { return ttfFont.get(); }

   private:
    std::shared_ptr<TTF_Font> ttfFont;
};

using FontPtr = std::shared_ptr<Font>;
}  // namespace sdl

#endif
