#ifndef __CURL_HH
#define __CURL_HH

#include <curl/curl.h>

#include <string>

namespace net {
class Curl {
   public:
    static void initialize();

   private:
    static bool initialized;
};

class Data {
   public:
    Data();
    ~Data();

    Data(const Data& o);
    Data& operator=(const Data& o);
    Data(Data&& o);
    Data& operator=(Data&& o);

    void reset();

    void append(uint8_t* src, size_t sz);
    size_t getLength() const { return length; }

    operator uint8_t*() const { return buffer; }
    operator const uint8_t*() const { return buffer; }
    operator void*() const { return buffer; }
    operator const void*() const { return buffer; }

    const uint8_t* get() const { return buffer; }

   private:
    static constexpr size_t initialSize = 128 * 1024;
    uint8_t* buffer;
    size_t size;
    size_t length;

    void grow();
    void free();
};

class Http {
   public:
    Http(const std::string& url, long connectTimeout = 2, long timeout = 30);
    ~Http();

    Http(const Http& o);
    Http& operator=(const Http& o);
    Http(Http&& o);
    Http& operator=(Http&& o);

    Data get(std::string& contentType);
    Data get();
    std::string getUrl() const { return url; }

   private:
    std::string url;
    CURL* connection;
    char errorBuffer[CURL_ERROR_SIZE];

    std::string getLastTransferContentType() const;
};
}  // namespace net

#endif
