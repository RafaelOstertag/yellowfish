#include "clock.hh"

#include <ctime>
#include <iomanip>
#include <sstream>

using namespace screens;

namespace {
std::string timeAsText(bool withSeconds) {
    auto seconds = std::time(nullptr);
    auto localTime = std::localtime(&seconds);

    std::stringstream ascTime;
    ascTime << std::setfill('0') << std::setw(2) << localTime->tm_hour << ':'
            << std::setfill('0') << std::setw(2) << localTime->tm_min;

    if (withSeconds) {
        ascTime << ':' << std::setfill('0') << std::setw(2)
                << localTime->tm_sec;
    }

    return ascTime.str();
}
}  // namespace

Clock::Clock(const std::string& fontpath, int size, const sdl::Color& fontColor,
             bool showSeconds)
    : font{new sdl::Font{fontpath, size}},
      color{fontColor},
      showSeconds{showSeconds} {}

void Clock::render(const sdl::Renderer& renderer) {
    auto timeText = timeAsText(showSeconds);
    auto texture = textToTexture(timeText, renderer);

    int textureWidth, textureHeight;
    SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);

    SDL_Rect viewPort;
    SDL_RenderGetViewport(renderer, &viewPort);

    SDL_Rect targetRect{(viewPort.w / 2) - (textureWidth / 2),
                        (viewPort.h / 2) - (textureHeight / 2), textureWidth,
                        textureHeight};

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    SDL_RenderCopy(renderer, texture, NULL, &targetRect);
    SDL_DestroyTexture(texture);
}

SDL_Texture* Clock::textToTexture(const std::string& text,
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
