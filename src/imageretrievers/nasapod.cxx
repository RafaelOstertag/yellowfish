#include "nasapod.hh"

#include <json/reader.h>

#include <sstream>

#include "../utils/imageresizer.hh"
#include "../utils/retriever_error.hh"

#ifndef NDEBUG
#include <iostream>
#endif

using namespace imageretriever;

namespace {

constexpr const char* baseUrl{"https://api.nasa.gov/planetary/apod?api_key="};

std::stringstream toStringStream(const net::Data& data) {
    std::stringstream stringStream;
    auto dataSize{data.get_length()};
    for (size_t i = 0; i < dataSize; i++) {
        stringStream << (data.get() + i);
    }
    return stringStream;
}
}  // namespace

NasaPod::NasaPod(int width, int height, const std::string& apiKey)
    : HttpImageRetriever{width, height} {
    std::stringstream url;
    url << baseUrl << apiKey;

    set_url(url.str());
}

screens::Image NasaPod::retrieve() {
    fetchFromNasa();
    SDL_RWseek(sdlMemory->getRWOps(), 0, RW_SEEK_SET);
    return screens::Image{*sdlMemory};
}

std::string NasaPod::getPictureOfTheDayUrl() const {
#ifndef NDEBUG
    std::cerr << "Retrieve URL of NASA Picture of the Day\n";
#endif
    auto rawMetaData = get_image();

    std::stringstream stringstream{toStringStream(rawMetaData)};

    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;

    Json::Value jsonRoot;
    JSONCPP_STRING errors;

    auto noError =
        Json::parseFromStream(builder, stringstream, &jsonRoot, &errors);
    if (!noError) {
        throw utils::RetrieverError(
            "Error retrieving NASA Picture of the Day meta data");
    }

    const Json::Value& urlValue = jsonRoot["url"];
    if (urlValue.isNull()) {
        throw utils::RetrieverError("No 'url' attribute in JSON");
    }
    return urlValue.asString();
}

void NasaPod::fetchFromNasa() {
    auto now{std::chrono::system_clock::now()};
    auto timeElapsed = now - lastRetrieved;

    if (!sdlMemory ||
        std::chrono::duration_cast<std::chrono::hours>(timeElapsed).count() >
            23) {
        std::string picUrl{getPictureOfTheDayUrl()};
#ifndef NDEBUG
        std::cerr << "Retrieving image from NASA\n";
#endif
        net::Http picRetriever{picUrl};
        auto pictureOfTheDay = picRetriever.get();

        sdlMemory = data_to_memory_rw_ops(pictureOfTheDay);
        lastRetrieved = now;
    }
#ifndef NDEBUG
    else {
        std::cerr << "Use cached image\n";
    }
#endif
}

std::unique_ptr<sdl::MemoryRWOps> NasaPod::data_to_memory_rw_ops(
    const net::Data& picture_data) const {
    try {
        Magick::Blob imageBlob{picture_data.get(), picture_data.get_length()};
        utils::ImageResizer imageResizer(imageBlob);
        auto resizedImage = imageResizer.resizeToMatch(
            HttpImageRetriever::width(), HttpImageRetriever::height());

        return std::make_unique<sdl::MemoryRWOps>(resizedImage->data(),
                                                  resizedImage->length());
    } catch (const std::exception& e) {
        throw utils::RetrieverError(e.what());
    }
}
