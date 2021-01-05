#include "fadein.hh"

using namespace screens;

namespace {
class NoOpFadeInCallback final : public FadeInCallback {
   public:
    void fadingIn() override {
        // It's no-op
    }
    void done() override {
        // It's no-op
    }
};
NoOpFadeInCallback NO_OP_FADEIN_CALLBACK{};
}  // namespace

FadeIn::FadeIn(const sdl::Color& fillColor, unsigned char fadeInStep)
    : Fill{fillColor},
      initialOpaqueColor{fillColor.red(), fillColor.green(), fillColor.blue(),
                         0xff},
      currentFillColor{initialOpaqueColor},
      fadeInCallback{NO_OP_FADEIN_CALLBACK},
      fadeInStep{fadeInStep} {}

FadeIn::FadeIn(const sdl::Color& fillColor, FadeInCallback& fadeInCallback,
               unsigned char fadeInStep)
    : Fill{fillColor},
      initialOpaqueColor{fillColor.red(), fillColor.green(), fillColor.blue(),
                         0xff},
      currentFillColor{initialOpaqueColor},
      fadeInCallback{fadeInCallback},
      fadeInStep{fadeInStep} {}

void FadeIn::render(const sdl::Renderer& renderer) {
    if (currentFillColor.alpha() == 0) {
        // there is no reason to draw it
        fadeInCallback.done();
        return;
    }
    setFillColor(currentFillColor);
    Fill::render(renderer);
    auto currentAlpha = currentFillColor.alpha();
    unsigned char newAlpha = currentAlpha - fadeInStep;
    currentFillColor = sdl::Color{
        currentFillColor.red(), currentFillColor.green(),
        currentFillColor.blue(),
        newAlpha > currentAlpha ? static_cast<unsigned char>(0) : newAlpha};
    fadeInCallback.fadingIn();
}
