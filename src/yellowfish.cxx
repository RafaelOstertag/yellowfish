#include "yellowfish.hh"

#include <algorithm>
#include <chrono>
#include <cstring>
#include <functional>
#include <iostream>
#include <random>
#include <sstream>
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

struct Config;

typedef screens::Image (*imageCreator_t)(const Config& config);

struct Config {
    bool fullScreen;
    int width;
    int height;
    std::vector<imageCreator_t> imageCreators;
};

std::unique_ptr<net::Http> picsum;
std::unique_ptr<net::Http> unsplash;
std::unique_ptr<net::NasaPod> nasaPod;

utils::DirectoryLister directoryLister{localImageDir};

screens::Image picsumImage(const Config& config) {
    auto data = picsum->get();

    sdl::MemoryRWOps memoryRWOps{data, data.getLength()};
    return screens::Image{memoryRWOps};
}

screens::Image unsplashImage(const Config& config) {
    auto data = picsum->get();

    sdl::MemoryRWOps memoryRWOps{data, data.getLength()};
    return screens::Image{memoryRWOps};
}

screens::Image nasaPictureOfTheDay(const Config& config) {
    auto data = nasaPod->fetch();

    Magick::Blob imageBlob{data, data.getLength()};
    utils::ImageResizer imageResizer(imageBlob);
    auto resizedImage = imageResizer.resize(config.width, config.height);

    sdl::MemoryRWOps memoryRWOps{resizedImage->data(), resizedImage->length()};
    return screens::Image{memoryRWOps};
}

screens::Image randomLocalImage(const Config& config) {
    auto list = directoryLister.list();

    if (list.empty()) {
        std::cerr << directoryLister.getPath() << " contains no files\n";
        return screens::Image();
    }

    auto imageIndex = std::bind(
        std::uniform_int_distribution<unsigned long>{0, list.size() - 1},
        std::mt19937(std::time(nullptr)));

    std::string localImageFile{list[imageIndex()]};
#ifndef NDEBUG
    std::cerr << "Loading local image " << localImageFile << '\n';
#endif
    utils::ImageResizer imageResizer(localImageFile);
    auto imageBlob = imageResizer.resize(config.width, config.height);
    sdl::MemoryRWOps memoryRWOps{imageBlob->data(), imageBlob->length()};
    return screens::Image{memoryRWOps};
}

screens::Image randomImage(const std::vector<imageCreator_t>& imageCreators,
                           const Config& config) {
    static auto screenSelector = std::bind(
        std::uniform_int_distribution<unsigned long>{0,
                                                     imageCreators.size() - 1},
        std::mt19937(std::time(nullptr)));

    try {
        imageCreator_t imageCreator{imageCreators[screenSelector()]};
        return imageCreator(config);
    } catch (std::exception& e) {
        std::cerr << e.what() << '\n';
        return screens::Image();
    }
}

void run(const Config& config) {
    sdl::Window window{"YellowFish", config.width, config.height, sdl::BLACK,
                       config.fullScreen};

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
            image = randomImage(config.imageCreators, config);
        }

        if (!image.isEmpty()) window.render(image);
        window.render(fill);
        window.render(clock);

        window.update();

        firstIteration = false;
        SDL_Delay(300);
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

        std::cerr << argv[0] << " [full] [local] [nasa] [unsplash] [picsum]\n";
        std::cerr << "\nfull\t\tfull screen\n"
                  << "local\t\tlocal random image from\n"
                  << "\t\t" << localImageDir << '\n'
                  << "nasa\t\tNASA picture of the day\n"
                  << "unsplash\tunsplash image\n"
                  << "picsum\t\tpicsum image\n";
        exit(1);
    }

    if (!selectedImageCreators.empty()) {
        config.imageCreators = selectedImageCreators;
    }
}

void configureWindowDimensions(Config& config) {
    if (config.fullScreen) {
        config.width = sdl::SDL::displayWidth();
        config.height = sdl::SDL::displayHeight();
        return;
    }

    config.width = 640;
    config.height = 480;
}

void setupImageRetrievers(const Config& config) {
    std::stringstream unsplashUrl;
    unsplashUrl << "https://source.unsplash.com/random/" << config.width << 'x'
                << config.height;
    unsplash = std::make_unique<net::Http>(unsplashUrl.str());

    std::stringstream picsumUrl;
    picsumUrl << "https://picsum.photos/";
    if (config.height == config.width) {
        picsumUrl << config.width;
    } else {
        picsumUrl << config.width << '/' << config.height;
    }
    picsum = std::make_unique<net::Http>(picsumUrl.str());

    nasaPod = std::make_unique<net::NasaPod>();
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
    configureWindowDimensions(config);
    setupImageRetrievers(config);

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
