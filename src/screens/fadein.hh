#ifndef __FADEIN_H
#define __FADEIN_H

#include "fill.hh"

namespace screens {

class FadeInCallback {
   public:
    virtual ~FadeInCallback() = default;
    virtual void fadingIn() = 0;
    virtual void done() = 0;
};

class FadeIn final : public Fill {
   public:
    FadeIn(const sdl::Color& fillColor, unsigned char fadeInStep = 10);
    FadeIn(const sdl::Color& fillColor, FadeInCallback& fadeInCallback,
           unsigned char fadeInStep = 10);

    void render(const sdl::Renderer& renderer) override;

    void reset() { currentFillColor = initialOpaqueColor; }

   private:
    sdl::Color initialOpaqueColor;
    sdl::Color currentFillColor;
    FadeInCallback& fadeInCallback;
    unsigned char fadeInStep;
};

}  // namespace screens

#endif
