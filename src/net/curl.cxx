#include "curl.hh"

#include <cstdlib>
#include <cstring>
#include <sstream>

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

Data::Data(size_t s) : size{s} {
    if (size == 0) {
        throw CurlError("Size must not be zero");
    }
}
void Data::reset() {
    buffer = std::make_unique<uint8_t[]>(initialSize);
    length = 0;
    size = initialSize;
}

void Data::append(const uint8_t* src, size_t sz) {
    auto newLength = length + sz;
    while (newLength > size) grow();

    std::memcpy(buffer.get() + length, src, sz);
    length = newLength;
}

void Data::grow() {
    size *= 2;

    auto new_buffer = std::make_unique<uint8_t[]>(size);
    std::memcpy(new_buffer.get(), buffer.get(), length);

    buffer.swap(new_buffer);

#ifndef NDEBUG
    std::cerr << "Grow buffer from " << size / 2 << " to " << size << '\n';
#endif
}

//
// class Http
//

namespace {
int curl_writer(char* data, size_t size, size_t nmemb, Data* buffer) {
    if (buffer == nullptr || nmemb == 0) return 0;

    buffer->append(reinterpret_cast<uint8_t*>(data), size * nmemb);

    return static_cast<int>(size * nmemb);
}
}  // namespace

Http::Http(const std::string& url, long connectTimeout, long timeout)
    : url{url} {
    Curl::initialize();
    connection = curl_easy_init();
    if (connection == nullptr) {
        throw CurlError("Unable to initialize cURL");
    }

    auto code = curl_easy_setopt(connection, CURLOPT_SSLVERSION,
                                 CURL_SSLVERSION_TLSv1_2);
    if (code != CURLE_OK) {
        std::stringstream msg;
        msg << "Failed to set TLS version: " << errorBuffer;
        throw CurlError(msg.str());
    }

    code = curl_easy_setopt(connection, CURLOPT_ERRORBUFFER, errorBuffer);
    if (code != CURLE_OK) {
        throw CurlError("Failed to set error buffer");
    }

    code = curl_easy_setopt(connection, CURLOPT_URL, url.c_str());
    if (code != CURLE_OK) {
        std::stringstream msg;
        msg << "Failed to set URL: " << errorBuffer;
        throw CurlError(msg.str());
    }

    code = curl_easy_setopt(connection, CURLOPT_FOLLOWLOCATION, 1L);
    if (code != CURLE_OK) {
        std::stringstream msg;
        msg << "Failed to set redirect option: " << errorBuffer;
        throw CurlError(msg.str());
    }

    code = curl_easy_setopt(connection, CURLOPT_WRITEFUNCTION, curl_writer);
    if (code != CURLE_OK) {
        std::stringstream msg;
        msg << "Failed to set writer: " << errorBuffer;
        throw CurlError(msg.str());
    }

    code = curl_easy_setopt(connection, CURLOPT_CONNECTTIMEOUT, connectTimeout);
    if (code != CURLE_OK) {
        std::stringstream msg;
        msg << "Failed to set connect timeout: " << errorBuffer;
        throw CurlError(msg.str());
    }

    code = curl_easy_setopt(connection, CURLOPT_TIMEOUT, timeout);
    if (code != CURLE_OK) {
        std::stringstream msg;
        msg << "Failed to set timeout: " << errorBuffer;
        throw CurlError(msg.str());
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

Http::Http(Http&& o) noexcept
    : url{std::move(o.url)}, connection{o.connection} {
    o.connection = nullptr;
}

Http& Http::operator=(Http&& o) noexcept {
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
        throw CurlError(msg.str());
    }

    code = curl_easy_perform(connection);
    if (code != CURLE_OK) {
        std::stringstream msg;
        msg << "Failed get '" << url << "': " << errorBuffer;
        throw CurlError(msg.str());
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
        throw CurlError(msg.str());
    }

    if (ct == nullptr) {
        return "";
    }

    return ct;
}
