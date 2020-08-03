#include "weather.hh"

using namespace screens;

Weather::Weather(const std::string& fontpath, int size,
                 const sdl::Color& fontColor)
    : weatherRetriever{},
      font{new sdl::Font{fontpath, size}},
      color{fontColor},
      background{static_cast<unsigned char>(0xff - fontColor.red()),
                 static_cast<unsigned char>(0xff - fontColor.green()),
                 static_cast<unsigned char>(0xff - fontColor.blue()), 64} {}

void Weather::render(const sdl::Renderer& renderer) {
    auto temperature = weatherRetriever.getTemperature();
    temperature += "C";

    auto texture = textToTexture(temperature, renderer);

    int textureWidth, textureHeight;
    SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
    SDL_Rect targetRect;
    targetRect.h = textureHeight;
    targetRect.w = textureWidth;
    targetRect.x = 0;
    targetRect.y = 0;

    SDL_SetRenderDrawColor(renderer, background.red(), background.green(),
                           background.blue(), background.alpha());

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(renderer, &targetRect);

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, texture, NULL, &targetRect);
    SDL_DestroyTexture(texture);
}

SDL_Texture* Weather::textToTexture(const std::string& text,
                                    const sdl::Renderer& renderer) {
    SDL_Surface* textSurface =
        TTF_RenderText_Blended(*font, text.c_str(), color);
    if (textSurface == nullptr) {
        std::string errorMsg{"Unable to render text surface: "};
        throw std::runtime_error(errorMsg + TTF_GetError());
    }

    auto texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (texture == nullptr) {
        std::string errorMsg{"Unable to create texture from rendered text: "};
        throw std::runtime_error(errorMsg + SDL_GetError());
    }

    SDL_FreeSurface(textSurface);
    return texture;
}
