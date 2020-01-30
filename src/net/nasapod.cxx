#include "nasapod.hh"

#include <json/reader.h>

#include <sstream>

#ifndef NDEBUG
#include <iostream>
#endif

using namespace net;

namespace {
std::stringstream toStringStream(const net::Data& data) {
    std::stringstream stringStream;
    auto dataSize{data.getLength()};
    for (size_t i = 0; i < dataSize; i++) {
        stringStream << (data.get() + i);
    }
    return stringStream;
}
}  // namespace

NasaPod::NasaPod(const std::string& apiKey)
    : metaDataFetcher{"https://api.nasa.gov/planetary/apod?api_key=" + apiKey},
      pictureOfTheDay{},
      lastRetrieved{} {}

net::Data NasaPod::fetch() {
    auto now{std::chrono::system_clock::now()};
    auto timeElapsed = now - lastRetrieved;

    if (pictureOfTheDay.getLength() == 0 ||
        std::chrono::duration_cast<std::chrono::hours>(timeElapsed).count() >
            23) {
        std::string picUrl{getPictureOfTheDayUrl()};
#ifndef NDEBUG
        std::cerr << "Retrieving image from NASA\n";
#endif
        net::Http picRetriever{picUrl};
        pictureOfTheDay = picRetriever.get();
        lastRetrieved = now;
    }
#ifndef NDEBUG
    else {
        std::cerr << "Use cached image\n";
    }
#endif

    return pictureOfTheDay;
}

std::string NasaPod::getPictureOfTheDayUrl() {
#ifndef NDEBUG
    std::cerr << "Retrieve URL of NASA Picture of the Day\n";
#endif
    auto rawMetaData = metaDataFetcher.get();
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
