#include "textrenderer.hh"

SDL_Texture* utils::textToTexture(const std::string& text,
                                  const sdl::Font& font,
                                  const sdl::Color& color,
                                  const sdl::Renderer& renderer) {
    SDL_Surface* textSurface = TTF_RenderText_Blended(
        font.get(), text.c_str(), static_cast<SDL_Color>(color));
    if (textSurface == nullptr) {
        std::string errorMsg{"Unable to render text surface: "};
        throw RenderError(errorMsg + TTF_GetError());
    }

    auto texture = SDL_CreateTextureFromSurface(renderer.get(), textSurface);
    if (texture == nullptr) {
        std::string errorMsg{"Unable to create texture from rendered text: "};
        throw RenderError(errorMsg + SDL_GetError());
    }

    SDL_FreeSurface(textSurface);
    return texture;
}
