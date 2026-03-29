#include "imageresizer.hh"

#include "../sdl/memoryrwops.hh"

using namespace utils;

ImageResizer::ImageResizer(const std::string &imagePath)
    : image{std::make_unique<Magick::Image>(imagePath)} {
}

ImageResizer::ImageResizer(const Magick::Blob &blob)
    : image{std::make_unique<Magick::Image>(blob)} {
}

std::shared_ptr<Magick::Blob> ImageResizer::resizeToMatch(unsigned int s) const {
    return resizeToMatch(s, s);
}

std::shared_ptr<Magick::Blob> ImageResizer::resizeToMatch(unsigned int x,
                                                          unsigned int y) const {
    Magick::Image workingImage = *image;

    auto width = static_cast<double>(workingImage.columns());
    auto height = static_cast<double>(workingImage.rows());

    if (x == y) {
        if (width > height) {
            auto scaleFactor = static_cast<double>(y) / height;
            workingImage.zoom(
                Magick::Geometry{static_cast<unsigned int>(width * scaleFactor), x});
        } else {
            auto scaleFactor = static_cast<double>(x) / width;
            workingImage.zoom(
                Magick::Geometry{x, static_cast<unsigned int>(height * scaleFactor)});
        }
    } else if (x > y) {
        auto scaleFactor = static_cast<double>(x) / width;
        workingImage.zoom(
            Magick::Geometry{x, static_cast<unsigned int>(height * scaleFactor)});
    } else {
        auto scaleFactor = static_cast<double>(y) / height;
        workingImage.zoom(
            Magick::Geometry{static_cast<unsigned int>(width * scaleFactor), y});
    }
    workingImage.crop(Magick::Geometry{x, y});

    auto blob = std::make_shared<Magick::Blob>();
    workingImage.write(blob.get());

    return blob;
}
