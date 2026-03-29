#ifndef __IMAGERESIZER_HH
#define __IMAGERESIZER_HH

#include <Magick++.h>

#include <memory>
#include <string>

namespace utils {
    class ImageResizer {
    public:
        explicit ImageResizer(const std::string &imagePath);

        explicit ImageResizer(const Magick::Blob &blob);

        /// Resize to a square
        std::shared_ptr<Magick::Blob> resizeToMatch(unsigned int s) const;

        std::shared_ptr<Magick::Blob> resizeToMatch(unsigned int x, unsigned int y) const;

    private:
        std::shared_ptr<Magick::Image> image;
    };
} // namespace utils

#endif
