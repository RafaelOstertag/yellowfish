#ifndef __CLOCK_HH
#define __CLOCK_HH

#include <stdexcept>
#include <string>

#include "../sdl/color.hh"
#include "../sdl/font.hh"
#include "../sdl/renderable.hh"

namespace screens {
class Clock final : public sdl::Renderable {
   public:
    enum class Alignment {
        TOP_RIGHT,
        TOP_LEFT,
        BOTTOM_RIGHT,
        BOTTOM_LEFT,
        CENTER
    };

    Clock(const std::string& fontpath, int size, const sdl::Color& fontColor,
          Alignment alignment = Alignment::CENTER, bool showSeconds = false);

    Clock(const Clock&) = delete;
    Clock(Clock&&) = delete;

    void render(const sdl::Renderer& renderer) override;

   private:
    sdl::FontPtr font;
    sdl::Color color;
    sdl::Color background;
    Alignment alignment;
    bool showSeconds;
};

class InvalidAlignmentError final : public std::invalid_argument {
   public:
    using std::invalid_argument::invalid_argument;
};
}  // namespace screens

#endif
