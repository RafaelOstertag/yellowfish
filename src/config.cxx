#include "config.hh"

#include <cstring>

#include "imageretrievers/imageserver.hh"
#include "imageretrievers/localfile.hh"
#include "imageretrievers/nasapod.hh"
#include "imageretrievers/picsum.hh"
#include "imageretrievers/unsplash.hh"
#include "sdl/sdl.hh"
#include "yellowfish.hh"

namespace {
bool wantFullScreen(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "full") == 0) {
            return true;
        }
    }

    return false;
}

int getWindowWidth(bool fullscreen) {
    return fullscreen ? sdl::SDL::displayWidth() : 720;
}

int getWindowHeight(bool fullscreen) {
    return fullscreen ? sdl::SDL::displayHeight() : 720;
}

screens::Clock::Alignment getAlignment(int argc, char** argv) {
    auto alignment = screens::Clock::Alignment::CENTER;

    for (int i = 1; i < argc; i++) {
        auto argument = argv[i];
        if (std::strcmp(argument, "center") == 0) {
            alignment = screens::Clock::Alignment::CENTER;
            continue;
        }

        if (std::strcmp(argument, "tl") == 0) {
            alignment = screens::Clock::Alignment::TOP_LEFT;
            continue;
        }

        if (std::strcmp(argument, "tr") == 0) {
            alignment = screens::Clock::Alignment::TOP_RIGHT;
            continue;
        }

        if (std::strcmp(argument, "bl") == 0) {
            alignment = screens::Clock::Alignment::BOTTOM_LEFT;
            continue;
        }

        if (std::strcmp(argument, "br") == 0) {
            alignment = screens::Clock::Alignment::BOTTOM_RIGHT;
            continue;
        }
    }
    return alignment;
}

int getPeriod(int argc, char** argv) {
    auto period = 60;
    for (int i = 1; i < argc; i++) {
        auto argument = argv[i];
        if (std::strcmp(argument, "45") == 0) {
            period = 45;
            continue;
        }

        if (std::strcmp(argument, "30") == 0) {
            period = 30;
            continue;
        }

        if (std::strcmp(argument, "15") == 0) {
            period = 15;
            continue;
        }

        if (std::strcmp(argument, "5") == 0) {
            period = 5;
            continue;
        }
    }
    return period;
}

std::vector<std::shared_ptr<imageretriever::ImageRetriever>> getImageRetrievers(
    int argc, char** argv, int width, int height) {
    auto imageRetrievers =
        std::vector<std::shared_ptr<imageretriever::ImageRetriever>>{};

    for (int i = 1; i < argc; i++) {
        auto argument = argv[i];
        if (std::strcmp(argument, "imageserver") == 0) {
            imageRetrievers.push_back(
                std::make_shared<imageretriever::ImageServer>(width, height));
            continue;
        }

        if (std::strcmp(argument, "nasa") == 0) {
            imageRetrievers.push_back(
                std::make_shared<imageretriever::NasaPod>(width, height));
            continue;
        }

        if (std::strcmp(argument, "unsplash") == 0) {
            imageRetrievers.push_back(
                std::make_shared<imageretriever::Unsplash>(width, height));
            continue;
        }

        if (std::strcmp(argument, "picsum") == 0) {
            imageRetrievers.push_back(
                std::make_shared<imageretriever::Picsum>(width, height));
            continue;
        }

        if (std::strcmp(argument, "local") == 0) {
            imageRetrievers.push_back(
                std::make_shared<imageretriever::LocalFile>(width, height,
                                                            localImageDir));
            continue;
        }
    }

    if (imageRetrievers.empty()) {
        imageRetrievers =
            std::vector<std::shared_ptr<imageretriever::ImageRetriever>>{
                std::make_shared<imageretriever::ImageServer>(width, height),
                std::make_shared<imageretriever::NasaPod>(width, height),
                std::make_shared<imageretriever::Unsplash>(width, height),
                std::make_shared<imageretriever::Picsum>(width, height)};
    }

    return imageRetrievers;
}

}  // namespace

namespace config {
void configure(int argc, char** argv, Config& config) {
    config.period = getPeriod(argc, argv);
    config.fullScreen = wantFullScreen(argc, argv);
    config.width = getWindowWidth(config.fullScreen);
    config.height = getWindowHeight(config.fullScreen);
    config.alignment = getAlignment(argc, argv);

    config.imageRetrievers =
        getImageRetrievers(argc, argv, config.width, config.height);

    for (int i = 1; i < argc; i++) {
        const char* argument{argv[i]};
        if (std::strcmp(argument, "help") == 0 ||
            std::strcmp(argument, "-h") == 0 ||
            std::strcmp(argument, "--help") == 0) {
            std::cerr << argv[0]
                      << " [full] [imageserver] [nasa] [unsplash] "
                         "[picsum] [local]\n\t[center|tl|tr|bl|br]\n";
            std::cerr << "\nfull\t\tfull screen\n"
                      << "imageserver\trandom image from image server\n"
                      << "nasa\t\tNASA picture of the day\n"
                      << "unsplash\tunsplash image\n"
                      << "picsum\t\tpicsum image\n"
                      << "local\t\tlocal random image from\n"
                      << "\t\t" << localImageDir << '\n'
                      << "center\t\tcenter align clock\n"
                      << "tl\t\talign clock top left\n"
                      << "tr\t\talign clock top right\n"
                      << "bl\t\talign clock bottom left\n"
                      << "br\t\talign clock bottom right\n"
                      << "5\t\t5s delay\n"
                      << "15\t\t15s delay\n"
                      << "30\t\t30s delay\n"
                      << "45\t\t45s delay\n";
            exit(1);
        }
    }
}
}  // namespace config
