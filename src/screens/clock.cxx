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

void alignCenter(const sdl::Renderer& renderer, SDL_Texture* texture,
                 SDL_Rect* targetRect) {
    int textureWidth, textureHeight;
    SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);

    SDL_Rect viewPort;
    SDL_RenderGetViewport(renderer, &viewPort);

    targetRect->x = (viewPort.w / 2) - (textureWidth / 2);
    targetRect->y = (viewPort.h / 2) - (textureHeight / 2);
    targetRect->w = textureWidth;
    targetRect->h = textureHeight;
}

void alignTopLeft(const sdl::Renderer& renderer, SDL_Texture* texture,
                  SDL_Rect* targetRect) {
    int textureWidth, textureHeight;
    SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);

    targetRect->x = 0;
    targetRect->y = 0;
    targetRect->w = textureWidth;
    targetRect->h = textureHeight;
}

void alignTopRight(const sdl::Renderer& renderer, SDL_Texture* texture,
                   SDL_Rect* targetRect) {
    int textureWidth, textureHeight;
    SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);

    SDL_Rect viewPort;
    SDL_RenderGetViewport(renderer, &viewPort);

    targetRect->x = viewPort.w - textureWidth;
    targetRect->y = 0;
    targetRect->w = textureWidth;
    targetRect->h = textureHeight;
}

void alignBottomLeft(const sdl::Renderer& renderer, SDL_Texture* texture,
                     SDL_Rect* targetRect) {
    int textureWidth, textureHeight;
    SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);

    SDL_Rect viewPort;
    SDL_RenderGetViewport(renderer, &viewPort);

    targetRect->x = 0;
    targetRect->y = viewPort.h - textureHeight;
    targetRect->w = textureWidth;
    targetRect->h = textureHeight;
}

void alignBottomRight(const sdl::Renderer& renderer, SDL_Texture* texture,
                      SDL_Rect* targetRect) {
    int textureWidth, textureHeight;
    SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);

    SDL_Rect viewPort;
    SDL_RenderGetViewport(renderer, &viewPort);

    targetRect->x = viewPort.w - textureWidth;
    targetRect->y = viewPort.h - textureHeight;
    targetRect->w = textureWidth;
    targetRect->h = textureHeight;
}

}  // namespace

Clock::Clock(const std::string& fontpath, int size, const sdl::Color& fontColor,
             Alignment alignment, bool showSeconds)
    : font{new sdl::Font{fontpath, size}},
      color{fontColor},
      alignment{alignment},
      showSeconds{showSeconds} {}

void Clock::render(const sdl::Renderer& renderer) {
    auto timeText = timeAsText(showSeconds);
    auto texture = textToTexture(timeText, renderer);

    SDL_Rect targetRect;
    switch (alignment) {
        case CENTER:
            alignCenter(renderer, texture, &targetRect);
            break;
        case TOP_RIGHT:
            alignTopRight(renderer, texture, &targetRect);
            break;
        case TOP_LEFT:
            alignTopLeft(renderer, texture, &targetRect);
            break;
        case BOTTOM_RIGHT:
            alignBottomRight(renderer, texture, &targetRect);
            break;
        case BOTTOM_LEFT:
            alignBottomLeft(renderer, texture, &targetRect);
            break;
    }

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
