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

namespace {

typedef screens::Image (*imageCreator_t)(void);

struct Config {
    bool fullScreen;
    std::vector<imageCreator_t> imageCreators;
};

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

screens::Image randomImage(const std::vector<imageCreator_t>& imageCreators) {
    static auto screenSelector = std::bind(
        std::uniform_int_distribution<unsigned long>{0,
                                                     imageCreators.size() - 1},
        std::mt19937(std::time(nullptr)));

    try {
        imageCreator_t imageCreator{imageCreators[screenSelector()]};
        return imageCreator();
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return screens::Image();
    }
}

void run(const Config& config) {
    sdl::Window window{"YellowFish", WINDOW_WIDTH_PX, WINDOW_HEIGHT_PX,
                       sdl::BLACK, config.fullScreen};

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
            image = randomImage(config.imageCreators);
        }

        if (!image.isEmpty()) window.render(image);
        window.render(fill);
        window.render(clock);

        window.update();

        firstIteration = false;
        SDL_Delay(10);
    }
}

void parseCommandLine(int argc, char** argv, Config& config) {
    config.fullScreen = false;
    config.imageCreators = std::vector<imageCreator_t>{
        randomLocalImage, nasaPictureOfTheDay, unsplashImage, picsumImage};

    if (argc == 1) {
        return;
    }

    std::vector<imageCreator_t> selectedImageCreators;
    for (int i = 1; i < argc; i++) {
        char* argument{argv[i]};
        if (std::strcmp(argument, "full") == 0) {
            config.fullScreen = true;
            continue;
        }

        if (std::strcmp(argument, "local") == 0) {
            selectedImageCreators.push_back(randomLocalImage);
            continue;
        }

        if (std::strcmp(argument, "nasa") == 0) {
            selectedImageCreators.push_back(nasaPictureOfTheDay);
            continue;
        }

        if (std::strcmp(argument, "unsplash") == 0) {
            selectedImageCreators.push_back(unsplashImage);
            continue;
        }

        if (std::strcmp(argument, "picsum") == 0) {
            selectedImageCreators.push_back(picsumImage);
            continue;
        }
    }

    if (!selectedImageCreators.empty()) {
        config.imageCreators = selectedImageCreators;
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

    Config config;
    parseCommandLine(argc, argv, config);

    try {
        run(config);
        return 0;
    } catch (std::exception& e) {
        std::cerr << "Ooops! " << e.what() << "\n";
        return 2;
    }

    sdl::SDL::shutdown();
}
