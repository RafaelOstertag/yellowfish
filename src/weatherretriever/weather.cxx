#include "weather.hh"

#include <iostream>
#include <sstream>
#include <vector>

// Refer to
// https://data.geo.admin.ch/ch.meteoschweiz.messwerte-aktuell/info/VQHA80_de.txt
// for a description of the response.
using namespace weatherretriever;

namespace {
std::vector<std::string> parseCsvLine(const std::string& line,
                                      char separator = ';') {
    std::vector<std::string> fields;
    fields.reserve(24);

    std::string::size_type lastPos = 0;
    std::string::size_type pos;
    for (pos = line.find(separator); pos != std::string::npos;
         pos = line.find(separator, pos + 1)) {
        fields.push_back(line.substr(lastPos, pos - lastPos));
        lastPos = pos + 1;
    }

    // treat last field
    if (lastPos < line.size())
        fields.push_back(line.substr(lastPos, line.size()));

    return fields;
}
}  // namespace

Weather::Weather() : http {
        "https://data.geo.admin.ch/ch.meteoschweiz.messwerte-aktuell/VQHA80.csv"
    }, timeKeeper{900}, currentInformation{"-"}, firstRun{true} {}

std::string Weather::getTemperature() {
    if (firstRun || timeKeeper.hasElapsed()) {
        fetchInformation();
    }
    firstRun = false;

    return currentInformation.temperature;
}

void Weather::fetchInformation() {
#ifndef NDEBUG
    std::cout << "Fetch weather information from " << http.getUrl() << "\n";
#endif

    net::Data data;
    try {
        data = http.get();
    } catch (std::runtime_error& e) {
        std::cerr << "Error retrieving weather information: " << e.what()
                  << "\n";
        return;
    }

    extractInformationFromData(data);
}

void Weather::extractInformationFromData(const net::Data& data) {
    std::stringstream ss;
    ss.write(reinterpret_cast<const char*>(data.get()), data.getLength());

    std::string line;
    std::getline(ss, line);
    for (; !ss.eof(); std::getline(ss, line)) {
        auto fields = parseCsvLine(line);

        if (fields.size() > 2 && fields[0] == "TAE") {
            currentInformation.temperature = fields[2];
            return;
        }
    }
}
