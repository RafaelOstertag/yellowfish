#ifndef __NASAPOD_HH
#define __NASAPOD_HH

#include "curl.hh"
#include <chrono>
#include <string>

namespace net {
class NasaPod {
  public:
    NasaPod(const std::string& apiKey = "DEMO_KEY");

    net::Data fetch();

  private:
    net::Http metaDataFetcher;
    net::Data pictureOfTheDay;
    std::chrono::system_clock::time_point lastRetrieved;

    std::string getPictureOfTheDayUrl();
};
} // namespace screens

#endif
