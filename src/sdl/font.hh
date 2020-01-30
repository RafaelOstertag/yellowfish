#ifndef __FONT_HH
#define __FONT_HH

#include <SDL_ttf.h>

#include <memory>
#include <string>

namespace sdl {

class Font {
   public:
    Font(const std::string& filepath, int size);
    ~Font();
    Font(const Font& o);
    Font& operator=(const Font& o);

    Font(Font&& other);
    Font& operator=(Font&& other);

    operator TTF_Font*() const { return ttfFont.get(); }

   private:
    std::shared_ptr<TTF_Font> ttfFont;
};

using FontPtr = std::shared_ptr<Font>;
}  // namespace sdl

#endif
