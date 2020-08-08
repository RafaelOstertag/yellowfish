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

}  // namespace

namespace config {
void configure(int argc, char** argv, Config& config) {
    config.period = 60;
    config.fullScreen = wantFullScreen(argc, argv);
    config.width = getWindowWidth(config.fullScreen);
    config.height = getWindowHeight(config.fullScreen);
    config.alignment = screens::Clock::CENTER;

    config.imageRetrievers =
        std::vector<std::shared_ptr<imageretriever::ImageRetriever>>{
            std::make_shared<imageretriever::ImageServer>(config.width,
                                                          config.height),
            std::make_shared<imageretriever::NasaPod>(config.width,
                                                      config.height),
            std::make_shared<imageretriever::Unsplash>(config.width,
                                                       config.height),
            std::make_shared<imageretriever::Picsum>(config.width,
                                                     config.height)};

    if (argc == 1) {
        return;
    }

    std::vector<std::shared_ptr<imageretriever::ImageRetriever>>
        selectedImageCreators;
    for (int i = 1; i < argc; i++) {
        char* argument{argv[i]};
        if (std::strcmp(argument, "full") == 0) {
            // This has already been handled
            continue;
        }

        if (std::strcmp(argument, "hiresonly") == 0) {
            // This has already been handled
            continue;
        }

        if (std::strcmp(argument, "center") == 0) {
            config.alignment = screens::Clock::CENTER;
            continue;
        }

        if (std::strcmp(argument, "tl") == 0) {
            config.alignment = screens::Clock::TOP_LEFT;
            continue;
        }

        if (std::strcmp(argument, "tr") == 0) {
            config.alignment = screens::Clock::TOP_RIGHT;
            continue;
        }

        if (std::strcmp(argument, "bl") == 0) {
            config.alignment = screens::Clock::BOTTOM_LEFT;
            continue;
        }

        if (std::strcmp(argument, "br") == 0) {
            config.alignment = screens::Clock::BOTTOM_RIGHT;
            continue;
        }

        if (std::strcmp(argument, "imageserver") == 0) {
            selectedImageCreators.push_back(
                std::make_shared<imageretriever::ImageServer>(config.width,
                                                              config.height));
            continue;
        }

        if (std::strcmp(argument, "nasa") == 0) {
            selectedImageCreators.push_back(
                std::make_shared<imageretriever::NasaPod>(config.width,
                                                          config.height));
            continue;
        }

        if (std::strcmp(argument, "unsplash") == 0) {
            selectedImageCreators.push_back(
                std::make_shared<imageretriever::Unsplash>(config.width,
                                                           config.height));
            continue;
        }

        if (std::strcmp(argument, "picsum") == 0) {
            selectedImageCreators.push_back(
                std::make_shared<imageretriever::Picsum>(config.width,
                                                         config.height));
            continue;
        }

        if (std::strcmp(argument, "local") == 0) {
            selectedImageCreators.push_back(
                std::make_shared<imageretriever::LocalFile>(
                    config.width, config.height, localImageDir));
            continue;
        }

        if (std::strcmp(argument, "45") == 0) {
            config.period = 45;
            continue;
        }

        if (std::strcmp(argument, "30") == 0) {
            config.period = 30;
            continue;
        }

        if (std::strcmp(argument, "15") == 0) {
            config.period = 15;
            continue;
        }

        if (std::strcmp(argument, "5") == 0) {
            config.period = 5;
            continue;
        }

        std::cerr << argv[0]
                  << " [full] [hiresonly] [imageserver] [nasa] [unsplash] "
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

    if (!selectedImageCreators.empty()) {
        config.imageRetrievers = selectedImageCreators;
    }
}
}  // namespace config
