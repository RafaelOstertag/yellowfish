#include "yellowfish.hh"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <functional>
#include <iostream>
#include <random>
#include <stdexcept>

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

    Magick::Blob imageBlob{data, data.getLength()};
    utils::ImageResizer imageResizer(imageBlob);
    auto resizedImage = imageResizer.resize(WINDOW_WIDTH_PX);

    sdl::MemoryRWOps memoryRWOps{resizedImage->data(), resizedImage->length()};
    return screens::Image{memoryRWOps};
}

screens::Image randomLocalImage() {
    auto list = directoryLister.list();

    if (list.empty()) {
        std::cerr << directoryLister.getPath() << " contains no files\n";
        return screens::Image();
    }

    auto imageIndex = std::bind(
        std::uniform_int_distribution<unsigned long>{0, list.size() - 1},
        std::mt19937(std::time(nullptr)));

    utils::ImageResizer imageResizer(list[imageIndex()]);
    auto imageBlob = imageResizer.resize(WINDOW_WIDTH_PX);
    sdl::MemoryRWOps memoryRWOps{imageBlob->data(), imageBlob->length()};
    return screens::Image{memoryRWOps};
}

screens::Image randomImage() {
    auto screenSelector =
        std::bind(std::uniform_int_distribution<unsigned long>{0, 3},
                  std::mt19937(std::time(nullptr)));

    try {
        switch (screenSelector()) {
            case 0:
#ifndef NDEBUG
                std::cerr << "retrieving Unsplash image\n";
#endif
                return unsplashImage();
            case 1:
#ifndef NDEBUG
                std::cerr << "retrieving Picsum image\n";
#endif
                return picsumImage();
            case 2:
#ifndef NDEBUG
                std::cerr << "retrieving random local image\n";
#endif
                return randomLocalImage();
            case 3:
#ifndef NDEBUG
                std::cerr << "retrieving NASA Picture of the Day\n";
#endif
                return nasaPictureOfTheDay();
            default:
                throw std::invalid_argument("Invalid image selection");
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return screens::Image();
    }
}

void run(bool fullscreen) {
    sdl::Window window{"YellowFish", WINDOW_WIDTH_PX, WINDOW_HEIGHT_PX,
                       sdl::BLACK, fullscreen};

    SDL_Event event;

    screens::Fill fill{sdl::Color{0x0, 0x0, 0x0, 0x80}};

    screens::Clock clock{clockFont, 200, sdl::Color{0xff, 0xff, 0xff, 0x90},
                         false};

    utils::TimeKeeper timeKeeper;

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
            image = randomImage();
        }

        if (!image.isEmpty()) window.render(image);
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
