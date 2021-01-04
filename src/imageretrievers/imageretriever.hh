#ifndef __IMAGERETRIEVER_HH
#define __IMAGERETRIEVER_HH

#include "../screens/image.hh"

namespace imageretriever {
class ImageRetriever {
   public:
    ImageRetriever(int width, int height) : _width{width}, _height{height} {}
    virtual ~ImageRetriever() = default;
    virtual screens::Image retrieve() = 0;

   protected:
    int width() const { return _width; }
    int height() const { return _height; }

   private:
    int _width;
    int _height;
};
}  // namespace imageretriever

#endif
