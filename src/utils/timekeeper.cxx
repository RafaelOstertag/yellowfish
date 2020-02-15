#include "timekeeper.hh"

using namespace utils;

TimeKeeper::TimeKeeper(int seconds) : period{seconds}, lastTimePoint{now()} {}

bool TimeKeeper::hasElapsed() {
    auto currentTime = now();
    auto difference = currentTime - lastTimePoint;

    auto secondsElapsed =
        std::chrono::duration_cast<std::chrono::seconds>(difference).count();

    if (secondsElapsed > period) {
        lastTimePoint = currentTime;
        return true;
    }
    return false;
}

inline TimeKeeper::time_point TimeKeeper::now() const {
    return std::chrono::system_clock::now();
}
