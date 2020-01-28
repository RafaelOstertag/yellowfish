#include "yellowfish.hh"
#include "net/curl.hh"
#include "net/nasapod.hh"
#include "screens/clock.hh"
#include "screens/fill.hh"
#include "screens/image.hh"
#include "sdl/memoryrwops.hh"
#include "sdl/sdl.hh"
#include "sdl/window.hh"
#include "utils/dirlist.hh"
#include "utils/imageresizer.hh"
#include "utils/timekeeper.hh"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <functional>
#include <iostream>
#include <random>
#include <stdexcept>

constexpr int WINDOW_WIDTH_PX{720};
constexpr int WINDOW_HEIGHT_PX{720};

net::Http picsum{"https://picsum.photos/720"};
net::Http unsplash{"https://source.unsplash.com/random/720x720"};
net::NasaPod nasaPod{};

utils::DirectoryLister directoryLister{localImageDir};

screens::Image picsumImage() {
    auto data = picsum.get();

    sdl::MemoryRWOps memoryRWOps{data, data.getLength()};
    return screens::Image{memoryRWOps};
}

screens::Image unsplashImage() {
    auto data = picsum.get();

    sdl::MemoryRWOps memoryRWOps{data, data.getLength()};
    return screens::Image{memoryRWOps};
}

screens::Image nasaPictureOfTheDay() {
    auto data = nasaPod.fetch();

    sdl::MemoryRWOps memoryRWOps{data, data.getLength()};
    return screens::Image{memoryRWOps};
}

screens::Image randomLocalImage() {
    auto list = directoryLister.list();

    std::random_shuffle(list.begin(), list.end());

    utils::ImageResizer imageResizer(list[0]);
    auto imageBlob = imageResizer.resize(WINDOW_WIDTH_PX);
    sdl::MemoryRWOps memoryRWOps{imageBlob->data(), imageBlob->length()};
    return screens::Image{memoryRWOps};
}

void run(bool fullscreen) {
    sdl::Window window{"YellowFish", WINDOW_WIDTH_PX, WINDOW_HEIGHT_PX,
                       sdl::BLACK, fullscreen};

    SDL_Event event;

    screens::Fill fill{sdl::Color{0x0, 0x0, 0x0, 0x80}};

    screens::Clock clock{clockFont, 200, sdl::Color{0xff, 0xff, 0xff, 0x90},
                         false};

    utils::TimeKeeper timeKeeper;

    auto imageSelector =
        std::bind(std::uniform_int_distribution<unsigned long>{0, 3},
                  std::mt19937(std::time(nullptr)));

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

        if (timeKeeper.hasMinuteElapsed() || firstIteration) {
            switch (imageSelector()) {
            case 0:
#ifndef NDEBUG
                std::cerr << "retrieving Unsplash image\n";
#endif
                image = unsplashImage();
                break;
            case 1:
#ifndef NDEBUG
                std::cerr << "retrieving Picsum image\n";
#endif
                image = picsumImage();
                break;
            case 2:
#ifndef NDEBUG
                std::cerr << "retrieving random local image\n";
#endif
                image = randomLocalImage();
                break;
            case 3:
#ifndef NDEBUG
                std::cerr << "retrieving NASA Picture of the Day\n";
#endif
                image = nasaPictureOfTheDay();
                break;

            default:
                throw std::invalid_argument("Invalid image selection");
            }
        }

        window.render(image);
        window.render(fill);
        window.render(clock);

        window.update();

        firstIteration = false;
        SDL_Delay(10);
    }
}

int main(int argc, char** argv) {
    Magick::InitializeMagick(nullptr);
    sdl::SDL::initialize();
    if (!sdl::SDL::isInitialized()) {
        return 1;
    }

    SDL_ShowCursor(SDL_DISABLE);

    bool fullscreen = false;
    if (argc == 2 && std::strcmp(argv[1], "full") == 0) {
        fullscreen = true;
    }

    try {
        run(fullscreen);
        return 0;
    } catch (std::exception& e) {
        std::cerr << "Ooops! " << e.what() << "\n";
        return 2;
    }

    sdl::SDL::shutdown();
}
