#ifndef __IMAGESERVER_HH
#define __IMAGESERVER_HH

#include "httpimageretriever.hh"

namespace imageretriever {
class ImageServer : public HttpImageRetriever {
   public:
    ImageServer(int width, int height, bool hiresOnly);
    virtual ~ImageServer() = default;
};
}  // namespace imageretriever

#endif