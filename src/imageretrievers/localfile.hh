#ifndef __LOCALFILE_HH
#define __LOCALFILE_HH

#include "../utils/dirlist.hh"
#include "imageretriever.hh"

namespace imageretriever {
class LocalFile final: public ImageRetriever {
   public:
    LocalFile(int width, int height, const std::string& directory)
        : ImageRetriever{width, height}, directoryLister{directory} {}

    screens::Image retrieve() override;

   private:
    utils::DirectoryLister directoryLister;
};

}  // namespace imageretriever

#endif
