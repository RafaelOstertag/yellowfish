#include "clock.hh"

#include <ctime>
#include <iomanip>
#include <sstream>

#include "screen_error.hh"
#include "../utils/textrenderer.hh"

using namespace screens;

namespace {
std::string timeAsText(bool withSeconds) {
    auto seconds = std::time(nullptr);
    tm time_struct;
    auto localTime = localtime_r(&seconds, &time_struct);

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
    int textureWidth;
    int textureHeight;
    SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

    SDL_Rect viewPort;
    SDL_RenderGetViewport(renderer.get(), &viewPort);

    targetRect->x = (viewPort.w / 2) - (textureWidth / 2);
    targetRect->y = (viewPort.h / 2) - (textureHeight / 2);
    targetRect->w = textureWidth;
    targetRect->h = textureHeight;
}

void alignTopLeft(SDL_Texture* texture,
                  SDL_Rect* targetRect) {
    int textureWidth;
    int textureHeight;
    SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

    targetRect->x = 0;
    targetRect->y = 0;
    targetRect->w = textureWidth;
    targetRect->h = textureHeight;
}

void alignTopRight(const sdl::Renderer& renderer, SDL_Texture* texture,
                   SDL_Rect* targetRect) {
    int textureWidth;
    int textureHeight;
    SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

    SDL_Rect viewPort;
    SDL_RenderGetViewport(renderer.get(), &viewPort);

    targetRect->x = viewPort.w - textureWidth;
    targetRect->y = 0;
    targetRect->w = textureWidth;
    targetRect->h = textureHeight;
}

void alignBottomLeft(const sdl::Renderer& renderer, SDL_Texture* texture,
                     SDL_Rect* targetRect) {
    int textureWidth;
    int textureHeight;
    SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

    SDL_Rect viewPort;
    SDL_RenderGetViewport(renderer.get(), &viewPort);

    targetRect->x = 0;
    targetRect->y = viewPort.h - textureHeight;
    targetRect->w = textureWidth;
    targetRect->h = textureHeight;
}

void alignBottomRight(const sdl::Renderer& renderer, SDL_Texture* texture,
                      SDL_Rect* targetRect) {
    int textureWidth;
    int textureHeight;
    SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

    SDL_Rect viewPort;
    SDL_RenderGetViewport(renderer.get(), &viewPort);

    targetRect->x = viewPort.w - textureWidth;
    targetRect->y = viewPort.h - textureHeight;
    targetRect->w = textureWidth;
    targetRect->h = textureHeight;
}

void renderTextBackground(const sdl::Renderer& renderer,
                          const SDL_Rect& textRect, const sdl::Color& color) {
    SDL_Rect viewPort;
    SDL_RenderGetViewport(renderer.get(), &viewPort);

    SDL_Rect backgroundRect;
    backgroundRect.h = textRect.h;
    backgroundRect.w = viewPort.w;
    backgroundRect.x = 0;
    backgroundRect.y = textRect.y;

    Uint8 savedRed;
    Uint8 savedGreen;
    Uint8 savedBlue;
    Uint8 savedAlpha;
    SDL_GetRenderDrawColor(renderer.get(), &savedRed, &savedGreen, &savedBlue,
                           &savedAlpha);
    SDL_SetRenderDrawColor(renderer.get(), color.red(), color.green(),
                           color.blue(), color.alpha());

    SDL_BlendMode savedBlendMode;
    SDL_GetRenderDrawBlendMode(renderer.get(), &savedBlendMode);

    SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);

    SDL_RenderFillRect(renderer.get(), &backgroundRect);

    SDL_SetRenderDrawColor(renderer.get(), savedRed, savedGreen, savedBlue,
                           savedAlpha);
    SDL_SetRenderDrawBlendMode(renderer.get(), savedBlendMode);
}

}  // namespace

Clock::Clock(const std::string& fontpath, int size, const sdl::Color& fontColor,
             Alignment alignment, bool showSeconds)
    : font{std::make_shared<sdl::Font>(fontpath, size)},
      color{fontColor},
      background{static_cast<unsigned char>(0xff - fontColor.red()),
                 static_cast<unsigned char>(0xff - fontColor.green()),
                 static_cast<unsigned char>(0xff - fontColor.blue()), 64},
      alignment{alignment},
      showSeconds{showSeconds} {}

void Clock::render(const sdl::Renderer& renderer) {
    auto timeText = timeAsText(showSeconds);
    auto texture = utils::textToTexture(timeText, *font.get(), color, renderer);

    SDL_Rect targetRect;
    switch (alignment) {
        case Alignment::CENTER:
            alignCenter(renderer, texture, &targetRect);
            break;
        case Alignment::TOP_RIGHT:
            alignTopRight(renderer, texture, &targetRect);
            break;
        case Alignment::TOP_LEFT:
            alignTopLeft(texture, &targetRect);
            break;
        case Alignment::BOTTOM_RIGHT:
            alignBottomRight(renderer, texture, &targetRect);
            break;
        case Alignment::BOTTOM_LEFT:
            alignBottomLeft(renderer, texture, &targetRect);
            break;
        default:
            throw InvalidAlignmentError("Alignment is not known");
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    renderTextBackground(renderer, targetRect, background);

    SDL_RenderCopy(renderer.get(), texture, nullptr, &targetRect);
    SDL_DestroyTexture(texture);
}
