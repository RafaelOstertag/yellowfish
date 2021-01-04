#ifndef __CURL_HH
#define __CURL_HH

#include <curl/curl.h>

#include <memory>
#include <stdexcept>
#include <string>

namespace net {

class CurlError final : public std::runtime_error {
   public:
    using std::runtime_error::runtime_error;
};

class Curl {
   public:
    static void initialize();

   private:
    static bool initialized;
};

class Data final {
   public:
    explicit Data(size_t s = initialSize);
    void reset();

    void append(const uint8_t* src, size_t sz);
    size_t get_length() const { return length; }

    const uint8_t* get() const { return buffer.get(); }

   private:
    static constexpr size_t initialSize = 128 * 1024;
    std::unique_ptr<uint8_t[]> buffer{std::make_unique<uint8_t[]>(initialSize)};
    size_t size{initialSize};
    size_t length{0};

    void grow();
};

class Http final {
   public:
    Http(const std::string& url, long connectTimeout = 2, long timeout = 30);
    ~Http();

    Http(const Http& o);
    Http& operator=(const Http& o);
    Http(Http&& o) noexcept;
    Http& operator=(Http&& o) noexcept;

    Data get(std::string& contentType);
    Data get();
    std::string getUrl() const { return url; }

   private:
    std::string url;
    CURL* connection{nullptr};
    char errorBuffer[CURL_ERROR_SIZE];

    std::string getLastTransferContentType() const;
};
}  // namespace net

#endif
