#include "unsplash.hh"

#include <sstream>

using namespace imageretriever;

namespace {
constexpr const char* baseUrl{"https://source.unsplash.com/random/"};
}  // namespace

Unsplash::Unsplash(int width, int height) : HttpImageRetriever{width, height} {
    std::stringstream unsplashUrl;
    unsplashUrl << baseUrl << HttpImageRetriever::width() << 'x'
                << HttpImageRetriever::height();
    set_url(unsplashUrl.str());
}
