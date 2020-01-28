#include "imageresizer.hh"
#include "../sdl/memoryrwops.hh"

using namespace utils;

ImageResizer::ImageResizer(const std::string& imagePath)
    : image{std::make_unique<Magick::Image>(imagePath)} {}

ImageResizer::~ImageResizer() {}

std::shared_ptr<Magick::Blob> ImageResizer::resize(size_t x) {
    Magick::Image workingImage = *image;

    auto width = workingImage.columns();
    auto height = workingImage.rows();

    if (width > height) {
        double scaleFactor = static_cast<double>(x) / height;
        workingImage.zoom(
            Magick::Geometry{static_cast<size_t>(width * scaleFactor), x});
    } else {
        double scaleFactor = static_cast<double>(x) / width;
        workingImage.zoom(
            Magick::Geometry{x, static_cast<size_t>(height * scaleFactor)});
    }
    workingImage.crop(Magick::Geometry{x, x});

    std::shared_ptr<Magick::Blob> blob = std::make_shared<Magick::Blob>();
    workingImage.write(blob.get());

    return blob;
}
