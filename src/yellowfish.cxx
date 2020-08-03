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
#include "screens/fill.hh"
#include "screens/image.hh"
#include "screens/weather.hh"
#include "sdl/memoryrwops.hh"
#include "sdl/sdl.hh"
#include "sdl/window.hh"
#include "utils/dirlist.hh"
#include "utils/imageresizer.hh"
#include "utils/timekeeper.hh"

namespace {

screens::Image randomImage(const config::Config& config) {
    static auto screenSelector = std::bind(
        std::uniform_int_distribution<unsigned long>{
            0, config.imageRetrievers.size() - 1},
        std::mt19937(std::time(nullptr)));

    try {
        auto retriever{config.imageRetrievers[screenSelector()]};
        return retriever->retrieve();
    } catch (std::exception& e) {
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

    bool firstIteration{true};
    screens::Image image;
    while (true) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                return;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_q:
                        return;
                }
            }
        }

        if (timeKeeper.hasElapsed() || firstIteration) {
            image = randomImage(config);
        }

        window.render(fill);
        if (!image.isEmpty()) window.render(image);

        window.render(clock);
        window.render(weather);

        window.update();

        firstIteration = false;
        SDL_Delay(300);
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
    } catch (std::exception& e) {
        std::cerr << "Ooops! " << e.what() << "\n";
        return 2;
    }

    sdl::SDL::shutdown();
}
