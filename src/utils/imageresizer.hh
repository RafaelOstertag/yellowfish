#ifndef __IMAGERESIZER_HH
#define __IMAGERESIZER_HH

#include <Magick++.h>
#include <memory>
#include <string>

namespace utils {
class ImageResizer {
  public:
    ImageResizer(const std::string& imagePath);
    ImageResizer(const Magick::Blob& blob);
    ~ImageResizer();

    std::shared_ptr<Magick::Blob> resize(size_t x);

  private:
    std::shared_ptr<Magick::Image> image;
};
} // namespace utils

#endif
