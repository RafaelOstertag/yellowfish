#include "picsum.hh"

#include <sstream>

using namespace imageretriever;

namespace {
const char* baseUrl{"https://picsum.photos/"};
}  // namespace

Picsum::Picsum(int width, int height) : HttpImageRetriever{width, height} {
    std::stringstream picsumUrl;
    picsumUrl << baseUrl;

    if (HttpImageRetriever::width() == HttpImageRetriever::height()) {
        picsumUrl << HttpImageRetriever::width();
    } else {
        picsumUrl << HttpImageRetriever::width() << '/'
                  << HttpImageRetriever::height();
    }

    http = std::make_unique<net::Http>(picsumUrl.str());
}
