#include "weather.hh"

#include "../utils/textrenderer.hh"
#include "screen_error.hh"

using namespace screens;

Weather::Weather(const std::string& fontpath, int size,
                 const sdl::Color& fontColor)
    : font{std::make_shared<sdl::Font>(fontpath, size)},
      color{fontColor},
      background{static_cast<unsigned char>(0xff - fontColor.red()),
                 static_cast<unsigned char>(0xff - fontColor.green()),
                 static_cast<unsigned char>(0xff - fontColor.blue()), 64} {}

void Weather::render(const sdl::Renderer& renderer) {
    auto temperature = weatherRetriever.getTemperature();
    temperature += "C";

    auto texture =
        utils::textToTexture(temperature, *font.get(), color, renderer);

    int textureWidth;
    int textureHeight;

    SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);
    SDL_Rect targetRect;
    targetRect.h = textureHeight;
    targetRect.w = textureWidth;
    targetRect.x = 0;
    targetRect.y = 0;

    SDL_SetRenderDrawColor(renderer.get(), background.red(), background.green(),
                           background.blue(), background.alpha());

    SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer.get(), &targetRect);

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer.get(), texture, nullptr, &targetRect);
    SDL_DestroyTexture(texture);
}
