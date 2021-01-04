#include "httpimageretriever.hh"

#include "../utils/retriever_error.hh"

using namespace imageretriever;

screens::Image HttpImageRetriever::retrieve() {
#ifndef NDEBUG
    std::cerr << "Loading image from " << http->getUrl() << '\n';
#endif
    auto data = get_image();

    sdl::MemoryRWOps memoryRWOps{data.get(), data.get_length()};
    return screens::Image{memoryRWOps};
}

net::Data HttpImageRetriever::get_image() const {
    try {
        return http->get();
    } catch (const net::CurlError& e) {
        throw utils::RetrieverError{e.what()};
    }
}
