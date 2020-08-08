#include "imageserver.hh"

#include <sstream>

using namespace imageretriever;

namespace {
const char* baseUrl{"http://images.kruemel.home:40005/images/"};
}  // namespace

ImageServer::ImageServer(int width, int height)
    : HttpImageRetriever{width, height} {
    std::stringstream imageServerUrl;
    imageServerUrl << baseUrl << HttpImageRetriever::width() << '/'
                   << HttpImageRetriever::height();
    http = std::make_unique<net::Http>(imageServerUrl.str());
}
