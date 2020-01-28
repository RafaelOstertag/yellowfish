#ifndef __TIMEKEEPER_HH
#define __TIMEKEEPER_HH

#include <chrono>

namespace utils {
class TimeKeeper {
  public:
    TimeKeeper();

    bool hasMinuteElapsed();
    bool hasSecondElapsed();

  private:
    using time_point = std::chrono::system_clock::time_point;
    time_point lastTimePointSeconds;
    time_point lastTimePointMinutes;

    time_point now() const;
};
} // namespace utils

#endif
