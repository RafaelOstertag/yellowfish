#ifndef __HTTPIMAGERETRIEVER_HH
#define __HTTPIMAGERETRIEVER_HH

#include <iostream>
#include <memory>

#include "../net/curl.hh"
#include "imageretriever.hh"

namespace imageretriever {
class HttpImageRetriever : public ImageRetriever {
   public:
    HttpImageRetriever(int width, int height)
        : ImageRetriever{width, height}, http{} {}
    virtual ~HttpImageRetriever() = default;

    virtual screens::Image retrieve() {
#ifndef NDEBUG
        std::cerr << "Loading image from " << http->getUrl() << '\n';
#endif
        auto data = http->get();

        sdl::MemoryRWOps memoryRWOps{data, data.getLength()};
        return screens::Image{memoryRWOps};
    }

   protected:
    std::unique_ptr<net::Http> http;
};
}  // namespace imageretriever

#endif
