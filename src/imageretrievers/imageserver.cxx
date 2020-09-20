#include "imageserver.hh"

#include <sstream>

using namespace imageretriever;

namespace {
const char* baseUrl{"http://imageserver.app.k8s.kruemel.home/images/"};
}  // namespace

ImageServer::ImageServer(int width, int height)
    : HttpImageRetriever{width, height} {
    std::stringstream imageServerUrl;
    imageServerUrl << baseUrl << HttpImageRetriever::width() << '/'
                   << HttpImageRetriever::height();
    http = std::make_unique<net::Http>(imageServerUrl.str());
}
