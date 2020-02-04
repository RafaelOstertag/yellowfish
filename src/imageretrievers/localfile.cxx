#include "localfile.hh"

#include <chrono>
#include <functional>
#include <iostream>
#include <random>

#include "../utils/imageresizer.hh"

using namespace imageretriever;

screens::Image LocalFile::retrieve() {
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
    auto imageBlob = imageResizer.resizeToMatch(ImageRetriever::width(),
                                                ImageRetriever::height());
    sdl::MemoryRWOps memoryRWOps{imageBlob->data(), imageBlob->length()};
    return screens::Image{memoryRWOps};
}
