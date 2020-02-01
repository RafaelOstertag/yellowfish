#include "imageresizer.hh"

#include "../sdl/memoryrwops.hh"

using namespace utils;

ImageResizer::ImageResizer(const std::string& imagePath)
    : image{std::make_unique<Magick::Image>(imagePath)} {}

ImageResizer::ImageResizer(const Magick::Blob& blob)
    : image{std::make_unique<Magick::Image>(blob)} {}

ImageResizer::~ImageResizer() {}

std::shared_ptr<Magick::Blob> ImageResizer::resize(size_t s) {
    return resize(s, s);
}

std::shared_ptr<Magick::Blob> ImageResizer::resize(size_t x, size_t y) {
    Magick::Image workingImage = *image;

    auto width = workingImage.columns();
    auto height = workingImage.rows();

    if (width > height) {
        double scaleFactor = static_cast<double>(y) / height;
        workingImage.zoom(
            Magick::Geometry{static_cast<size_t>(width * scaleFactor), y});
    } else {
        double scaleFactor = static_cast<double>(x) / width;
        workingImage.zoom(
            Magick::Geometry{x, static_cast<size_t>(height * scaleFactor)});
    }
    workingImage.crop(Magick::Geometry{x, y});

    std::shared_ptr<Magick::Blob> blob = std::make_shared<Magick::Blob>();
    workingImage.write(blob.get());

    return blob;
}
