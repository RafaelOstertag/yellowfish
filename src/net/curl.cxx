#include "curl.hh"

#include <cstdlib>
#include <cstring>
#include <sstream>
#include <stdexcept>

#ifndef NDEBUG
#include <iostream>
#endif

using namespace net;

//
// class Curl
//

bool Curl::initialized{false};

void Curl::initialize() {
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        initialized = true;
    }
}

//
// class Data
//

Data::Data()
    : buffer{static_cast<uint8_t*>(std::malloc(initialSize))},
      size{initialSize}, length{0} {
    if (buffer == nullptr) {
        throw std::runtime_error("Out of memory while initializing");
    }
}

Data::~Data() { free(); }

Data::Data(const Data& o)
    : buffer{static_cast<uint8_t*>(std::malloc(o.size))}, size{o.size},
      length{o.length} {
    if (buffer == nullptr) {
        throw std::runtime_error("Out of memory while copying");
    }

    std::memcpy(buffer, o.buffer, length);
}

Data& Data::operator=(const Data& o) {
    if (&o == this) {
        return *this;
    }

    free();
    buffer = static_cast<uint8_t*>(std::malloc(o.size));
    if (buffer == nullptr) {
        throw std::runtime_error("Out of memory while copy-assigning");
    }
    size = o.size;
    length = o.length;

    std::memcpy(buffer, o.buffer, length);

    return *this;
}

Data::Data(Data&& o) : buffer{o.buffer}, size{o.size}, length{o.length} {
    o.buffer = nullptr;
    o.size = 0;
    o.length = 0;
}

Data& Data::operator=(Data&& o) {
    if (&o == this) {
        return *this;
    }

    free();

    buffer = o.buffer;
    size = o.size;
    length = o.length;

    o.buffer = nullptr;
    o.size = 0;
    o.length = 0;

    return *this;
}

void Data::reset() {
    auto newBuffer = static_cast<uint8_t*>(malloc(initialSize));
    if (newBuffer == nullptr) {
        throw std::runtime_error("Out of memory");
    }

    free();

    buffer = newBuffer;
    length = 0;
    size = initialSize;
}

void Data::append(uint8_t* src, size_t sz) {
    auto newLength = length + sz;
    if (newLength > size)
        grow();

    std::memcpy(buffer + length, src, sz);
    length = newLength;
}

void Data::grow() {
    size *= 2;

    buffer = static_cast<uint8_t*>(std::realloc(buffer, size));
    if (buffer == nullptr) {
        throw std::runtime_error("Out of memory while growing");
    }
#ifndef NDEBUG
    std::cerr << "Grow buffer from " << size / 2 << " to " << size << '\n';
#endif
}

void Data::free() {
    if (buffer != nullptr) {
        std::free(buffer);
        buffer = nullptr;
    }
}

//
// class Http
//

namespace {
int curl_writer(char* data, size_t size, size_t nmemb, Data* buffer) {
    if (buffer == NULL || nmemb == 0)
        return 0;

    buffer->append(reinterpret_cast<uint8_t*>(data), size * nmemb);

    return size * nmemb;
}
} // namespace

Http::Http(const std::string& url) : url{url}, connection{nullptr} {
    Curl::initialize();
    connection = curl_easy_init();
    if (connection == nullptr) {
        throw std::runtime_error("Unable to initialize cURL");
    }

    auto code = curl_easy_setopt(connection, CURLOPT_ERRORBUFFER, errorBuffer);
    if (code != CURLE_OK) {
        throw std::runtime_error("Failed to set error buffer");
    }

    code = curl_easy_setopt(connection, CURLOPT_URL, url.c_str());
    if (code != CURLE_OK) {
        std::stringstream msg;
        msg << "Failed to set URL: " << errorBuffer;
        throw std::runtime_error(msg.str());
    }

    code = curl_easy_setopt(connection, CURLOPT_FOLLOWLOCATION, 1L);
    if (code != CURLE_OK) {
        std::stringstream msg;
        msg << "Failed to set redirect option: " << errorBuffer;
        throw std::runtime_error(msg.str());
    }

    code = curl_easy_setopt(connection, CURLOPT_WRITEFUNCTION, curl_writer);
    if (code != CURLE_OK) {
        std::stringstream msg;
        msg << "Failed to set writer: " << errorBuffer;
        throw std::runtime_error(msg.str());
    }
}

Http::~Http() {
    if (connection != nullptr) {
        curl_easy_cleanup(connection);
        connection = nullptr;
    }
}

Http::Http(const Http& o)
    : url{o.url}, connection{curl_easy_duphandle(o.connection)} {}

Http& Http::operator=(const Http& o) {
    if (&o == this) {
        return *this;
    }

    url = o.url;

    curl_easy_cleanup(connection);
    connection = curl_easy_duphandle(o.connection);

    return *this;
}

Http::Http(Http&& o) : url{o.url}, connection{o.connection} {
    o.connection = nullptr;
}

Http& Http::operator=(Http&& o) {
    if (&o == this) {
        return *this;
    }

    url = o.url;

    curl_easy_cleanup(connection);
    connection = o.connection;
    o.connection = nullptr;

    return *this;
}

Data Http::get(std::string& contentType) {
    Data dataBuffer;
    auto code = curl_easy_setopt(connection, CURLOPT_WRITEDATA, &dataBuffer);
    if (code != CURLE_OK) {
        std::stringstream msg;
        msg << "Failed to set write data: " << errorBuffer;
        throw std::runtime_error(msg.str());
    }

    code = curl_easy_perform(connection);
    if (code != CURLE_OK) {
        std::stringstream msg;
        msg << "Failed get '" << url << "': " << errorBuffer;
        throw std::runtime_error(msg.str());
    }

    contentType = getLastTransferContentType();

    return dataBuffer;
}

Data Http::get() {
    std::string wdc;
    return get(wdc);
}

std::string Http::getLastTransferContentType() const {
    char* ct = nullptr;
    auto res = curl_easy_getinfo(connection, CURLINFO_CONTENT_TYPE, &ct);
    if (res != CURLE_OK) {
        std::stringstream msg;
        msg << "Error getting content type: " << errorBuffer;
        throw std::runtime_error(msg.str());
    }

    if (ct == nullptr) {
        return "";
    }

    return ct;
}
