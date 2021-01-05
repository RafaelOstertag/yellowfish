#ifndef __WEATHER_HH
#define __WEATHER_HH

#include "../sdl/color.hh"
#include "../sdl/font.hh"
#include "../sdl/renderable.hh"
#include "../weatherretriever/weather.hh"

namespace screens {
class Weather final : public sdl::Renderable {
   public:
    Weather(const std::string& fontpath, int size, const sdl::Color& fontColor);

    void render(const sdl::Renderer& renderer) override;

   private:
    weatherretriever::Weather weatherRetriever{};
    sdl::FontPtr font;
    sdl::Color color;
    sdl::Color background;
};
}  // namespace screens

#endif
