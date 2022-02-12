#include "imageserver.hh"

#include <sstream>

using namespace imageretriever;

namespace {
constexpr const char* baseUrl{"https://imageserver.app.k8s.kruemel.home/images/"};
}  // namespace

ImageServer::ImageServer(int width, int height)
    : HttpImageRetriever{width, height} {
    std::stringstream imageServerUrl;
    imageServerUrl << baseUrl << HttpImageRetriever::width() << '/'
                   << HttpImageRetriever::height();
    set_url(imageServerUrl.str());
}
