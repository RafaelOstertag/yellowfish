#ifndef __UNSPLASH_HH
#define __UNSPLASH_HH

#include <memory>
#include <string>

#include "httpimageretriever.hh"

namespace imageretriever {
class Unsplash : public HttpImageRetriever {
   public:
    Unsplash(int width, int height);
    virtual ~Unsplash() = default;
};
}  // namespace imageretriever

#endif
