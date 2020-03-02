#include "nasapod.hh"

#include <json/reader.h>

#include <sstream>

#include "../utils/imageresizer.hh"

#ifndef NDEBUG
#include <iostream>
#endif

using namespace imageretriever;

namespace {

const char* baseUrl{"https://api.nasa.gov/planetary/apod?api_key="};

std::stringstream toStringStream(const net::Data& data) {
    std::stringstream stringStream;
    auto dataSize{data.getLength()};
    for (size_t i = 0; i < dataSize; i++) {
        stringStream << (data.get() + i);
    }
    return stringStream;
}
}  // namespace

NasaPod::NasaPod(int width, int height, const std::string& apiKey)
    : HttpImageRetriever{width, height}, sdlMemory{}, lastRetrieved{} {
    std::stringstream url;
    url << baseUrl << apiKey;

    http = std::make_unique<net::Http>(url.str());
}

screens::Image NasaPod::retrieve() {
    fetchFromNasa();
    SDL_RWseek(sdlMemory->getRWOps(), 0, RW_SEEK_SET);
    return screens::Image{*sdlMemory};
}

std::string NasaPod::getPictureOfTheDayUrl() {
#ifndef NDEBUG
    std::cerr << "Retrieve URL of NASA Picture of the Day\n";
#endif
    auto rawMetaData = http->get();
    std::stringstream stringstream{toStringStream(rawMetaData)};

    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;

    Json::Value jsonRoot;
    JSONCPP_STRING errors;

    auto noError =
        Json::parseFromStream(builder, stringstream, &jsonRoot, &errors);
    if (!noError) {
        throw std::runtime_error(
            "Error retrieving NASA Picture of the Day meta data");
    }

    auto urlValue = jsonRoot["url"];
    if (urlValue.isNull()) {
        throw std::runtime_error("No 'url' attribute in JSON");
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

        Magick::Blob imageBlob{pictureOfTheDay, pictureOfTheDay.getLength()};
        utils::ImageResizer imageResizer(imageBlob);
        auto resizedImage = imageResizer.resizeToMatch(
            HttpImageRetriever::width(), HttpImageRetriever::height());

        sdlMemory = std::make_unique<sdl::MemoryRWOps>(resizedImage->data(),
                                                       resizedImage->length());

        lastRetrieved = now;
    }
#ifndef NDEBUG
    else {
        std::cerr << "Use cached image\n";
    }
#endif
}
