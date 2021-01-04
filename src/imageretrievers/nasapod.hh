#ifndef __NASAPOD_HH
#define __NASAPOD_HH

#include <chrono>
#include <string>

#include "httpimageretriever.hh"

namespace imageretriever {
class NasaPod final : public HttpImageRetriever {
   public:
    NasaPod(int width, int height, const std::string& apiKey = "DEMO_KEY");

    screens::Image retrieve() override;

   private:
    std::unique_ptr<sdl::MemoryRWOps> sdlMemory{};
    std::chrono::system_clock::time_point lastRetrieved{};

    std::string getPictureOfTheDayUrl() const;
    void fetchFromNasa();
    std::unique_ptr<sdl::MemoryRWOps> data_to_memory_rw_ops(
        const net::Data& picture_data) const;
};
}  // namespace imageretriever

#endif
