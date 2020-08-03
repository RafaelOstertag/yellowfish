#ifndef __WEATHERRETRIEVER_HH
#define __WEATHERRETRIEVER_HH

#include <iostream>
#include <memory>
#include <string>

#include "../net/curl.hh"
#include "../utils/timekeeper.hh"

namespace weatherretriever {
class Weather {
   public:
    Weather();
    virtual ~Weather() = default;

    std::string getTemperature();

   private:
    struct WeatherInformation {
        std::string temperature;
    };

    net::Http http;
    utils::TimeKeeper timeKeeper;
    WeatherInformation currentInformation;
    bool firstRun;

    void fetchInformation();
    void extractInformationFromData(const net::Data& data);
};

}  // namespace weatherretriever

#endif
