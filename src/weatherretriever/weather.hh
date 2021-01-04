#ifndef __WEATHERRETRIEVER_HH
#define __WEATHERRETRIEVER_HH

#include <iostream>
#include <memory>
#include <string>

#include "../net/curl.hh"
#include "../utils/timekeeper.hh"

namespace weatherretriever {

class Weather final {
   public:
    Weather() = default;

    std::string getTemperature();

   private:
    struct WeatherInformation {
        std::string temperature;
    };

    net::Http http{
        "https://data.geo.admin.ch/ch.meteoschweiz.messwerte-aktuell/"
        "VQHA80.csv"};
    utils::TimeKeeper timeKeeper{900};
    WeatherInformation currentInformation{"-"};
    bool firstRun{true};

    void fetchInformation();
    void extractInformationFromData(const net::Data& data);
};

}  // namespace weatherretriever

#endif
