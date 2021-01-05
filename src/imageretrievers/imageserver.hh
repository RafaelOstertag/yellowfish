#ifndef __IMAGESERVER_HH
#define __IMAGESERVER_HH

#include "httpimageretriever.hh"

namespace imageretriever {
class ImageServer final : public HttpImageRetriever {
   public:
    ImageServer(int width, int height);
};
}  // namespace imageretriever

#endif
