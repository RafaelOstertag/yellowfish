#include "yellowfish.hh"

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>

#include "config.hh"
#include "imageretrievers/nasapod.hh"
#include "net/curl.hh"
#include "screens/clock.hh"
#include "screens/fadein.hh"
#include "screens/fill.hh"
#include "screens/image.hh"
#include "screens/weather.hh"
#include "sdl/memoryrwops.hh"
#include "sdl/sdl.hh"
#include "sdl/window.hh"
#include "utils/dirlist.hh"
#include "utils/imageresizer.hh"
#include "utils/retriever_error.hh"
#include "utils/timekeeper.hh"

namespace {

constexpr int DELAY_DURING_FADEIN{15};
constexpr int DELAY_NO_FADEIN_IN_PROGRESS{300};

class FadeInDelayAdjuster final : public screens::FadeInCallback {
   public:
    explicit FadeInDelayAdjuster(int& delayReference)
        : delayReference{delayReference} {}
    void fadingIn() override { delayReference = DELAY_DURING_FADEIN; }
    void done() override { delayReference = DELAY_NO_FADEIN_IN_PROGRESS; }

   private:
    int& delayReference;
};

screens::Image randomImage(const config::Config& config) {
    static auto screenSelector = std::bind(
        std::uniform_int_distribution<unsigned long>{
            0, config.imageRetrievers.size() - 1},
        std::mt19937(std::time(nullptr)));

    try {
        auto retriever{config.imageRetrievers[screenSelector()]};
        return retriever->retrieve();
    } catch (const utils::RetrieverError& e) {
        std::cerr << e.what() << '\n';
        return screens::Image();
    }
}  // namespace

void run(const config::Config& config) {
    sdl::Window window{"YellowFish", config.width, config.height, sdl::BLACK,
                       config.fullScreen};

    SDL_Event event;

    screens::Fill fill{sdl::Color{0x0, 0x0, 0x0, 0xff}};

    screens::Clock clock{clockFont, 200, sdl::Color{0xff, 0xff, 0xff, 0xd0},
                         config.alignment};
    screens::Weather weather{clockFont, 75, sdl::Color{0xff, 0xff, 0xff, 0xd0}};

    utils::TimeKeeper timeKeeper{config.period};

    weatherretriever::Weather weatherRetriever;

    int delay = DELAY_NO_FADEIN_IN_PROGRESS;
    FadeInDelayAdjuster fadeInDelayAdjuster{delay};
    screens::FadeIn fadeIn{sdl::Color{0x0, 0x0, 0x0, 0xff}, fadeInDelayAdjuster,
                           25};

    bool firstIteration{true};
    screens::Image image;
    while (true) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                return;
            } else if (event.type == SDL_KEYDOWN &&
                       event.key.keysym.sym == SDLK_q) {
                return;
            }
        }

        if (timeKeeper.hasElapsed() || firstIteration) {
            image = randomImage(config);
            fadeIn.reset();
        }

        window.render(fill);
        if (!image.isEmpty()) window.render(image);
        window.render(fadeIn);

        window.render(clock);
        window.render(weather);

        window.update();

        firstIteration = false;
        SDL_Delay(delay);
    }
}

}  // namespace

int main(int argc, char** argv) {
    Magick::InitializeMagick(nullptr);
    sdl::SDL::initialize();
    if (!sdl::SDL::isInitialized()) {
        return 1;
    }

    SDL_ShowCursor(SDL_DISABLE);

    config::Config config;
    config::configure(argc, argv, config);

    std::cout << "Window dimension " << config.width << 'x' << config.height
              << '\n';
    std::cout << "Fullscreen mode " << (config.fullScreen ? "on" : "off")
              << '\n';

    try {
        run(config);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Ooops! " << e.what() << "\n";
        return 2;
    }

    sdl::SDL::shutdown();
}
