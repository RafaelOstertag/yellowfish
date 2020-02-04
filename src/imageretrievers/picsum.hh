#ifndef __PICSUM_HH
#define __PICSUM_HH

#include "httpimageretriever.hh"

namespace imageretriever {
class Picsum : public HttpImageRetriever {
   public:
    Picsum(int width, int height);
    virtual ~Picsum() {}
};
}  // namespace imageretriever

#endif
