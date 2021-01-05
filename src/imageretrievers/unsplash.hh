#ifndef __UNSPLASH_HH
#define __UNSPLASH_HH

#include <memory>
#include <string>

#include "httpimageretriever.hh"

namespace imageretriever {
class Unsplash final : public HttpImageRetriever {
   public:
    Unsplash(int width, int height);
};
}  // namespace imageretriever

#endif
