#include "timekeeper.hh"

using namespace utils;

TimeKeeper::TimeKeeper()
    : lastTimePointSeconds{now()}, lastTimePointMinutes{lastTimePointSeconds} {}

bool TimeKeeper::hasMinuteElapsed() {
    auto currentTime = now();
    auto difference = currentTime - lastTimePointMinutes;

    auto minutesElapsed =
        std::chrono::duration_cast<std::chrono::minutes>(difference).count();

    if (minutesElapsed > 0) {
        lastTimePointMinutes = currentTime;
        return true;
    }
    return false;
}

bool TimeKeeper::hasSecondElapsed() {
    auto currentTime = now();
    auto difference = currentTime - lastTimePointSeconds;

    auto secondsElapsed =
        std::chrono::duration_cast<std::chrono::seconds>(difference).count();

    if (secondsElapsed > 0) {
        lastTimePointSeconds = currentTime;
        return true;
    }
    return false;
}

inline TimeKeeper::time_point TimeKeeper::now() const {
    return std::chrono::system_clock::now();
}
