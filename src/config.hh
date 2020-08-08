#ifndef __CONFIG_HH
#define __CONFIG_HH

#include <memory>
#include <vector>

#include "imageretrievers/imageretriever.hh"
#include "screens/clock.hh"

namespace config {

struct Config {
    int period;
    int width;
    int height;
    std::vector<std::shared_ptr<imageretriever::ImageRetriever>>
        imageRetrievers;
    screens::Clock::Alignment alignment;
    bool fullScreen;
};

extern void configure(int argc, char** argv, Config& config);

}  // namespace config

#endif
