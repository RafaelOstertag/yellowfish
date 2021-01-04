#ifndef __PICSUM_HH
#define __PICSUM_HH

#include "httpimageretriever.hh"

namespace imageretriever {
class Picsum final : public HttpImageRetriever {
   public:
    Picsum(int width, int height);
};
}  // namespace imageretriever

#endif
