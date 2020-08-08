#ifndef __FADEIN_H
#define __FADEIN_H

#include "fill.hh"

namespace screens {
class FadeIn : public Fill {
   public:
    FadeIn(const sdl::Color& fillColor, unsigned char fadeInStep = 10)
        : Fill{fillColor},
          initialOpaqueColor{fillColor.red(), fillColor.green(),
                             fillColor.blue(), 0xff},
          currentFillColor{initialOpaqueColor},
          fadeInStep{fadeInStep} {}
    virtual ~FadeIn() = default;

    virtual void render(const sdl::Renderer& renderer) {
        if (currentFillColor.alpha() == 0) {
            // there is no reason to draw it
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
    }

    void reset() { currentFillColor = initialOpaqueColor; }

   private:
    sdl::Color initialOpaqueColor;
    sdl::Color currentFillColor;
    unsigned char fadeInStep;
};
}  // namespace screens

#endif
