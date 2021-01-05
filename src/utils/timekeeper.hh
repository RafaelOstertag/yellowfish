#ifndef __TIMEKEEPER_HH
#define __TIMEKEEPER_HH

#include <chrono>

namespace utils {
class TimeKeeper {
   public:
    explicit TimeKeeper(int seconds);

    bool hasElapsed();

   private:
    using time_point = std::chrono::system_clock::time_point;
    int period;
    time_point lastTimePoint;

    time_point now() const;
};
}  // namespace utils

#endif
