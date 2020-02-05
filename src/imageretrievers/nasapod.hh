#ifndef __NASAPOD_HH
#define __NASAPOD_HH

#include <chrono>
#include <string>

#include "httpimageretriever.hh"

namespace imageretriever {
class NasaPod : public HttpImageRetriever {
   public:
    NasaPod(int width, int height, const std::string& apiKey = "DEMO_KEY");
    virtual ~NasaPod() = default;

    virtual screens::Image retrieve();

   private:
    std::unique_ptr<sdl::MemoryRWOps> sdlMemory;
    std::chrono::system_clock::time_point lastRetrieved;

    std::string getPictureOfTheDayUrl();
    void fetchFromNasa();
};
}  // namespace imageretriever

#endif
