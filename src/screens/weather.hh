#ifndef __WEATHER_HH
#define __WEATHER_HH

#include "../sdl/color.hh"
#include "../sdl/font.hh"
#include "../sdl/renderable.hh"
#include "../weatherretriever/weather.hh"

namespace screens {
class Weather : public sdl::Renderable {
   public:
    Weather(const std::string& fontpath, int size, const sdl::Color& fontColor);
    virtual ~Weather() = default;

    virtual void render(const sdl::Renderer& renderer);

   private:
    weatherretriever::Weather weatherRetriever;
    sdl::FontPtr font;
    sdl::Color color;
    sdl::Color background;

    SDL_Texture* textToTexture(const std::string& text,
                               const sdl::Renderer& renderer);
};
}  // namespace screens

#endif
