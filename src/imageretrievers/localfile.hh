#ifndef __LOCALFILE_HH
#define __LOCALFILE_HH

#include "../utils/dirlist.hh"
#include "imageretriever.hh"

namespace imageretriever {
class LocalFile : public ImageRetriever {
   public:
    LocalFile(int width, int height, const std::string& directory)
        : ImageRetriever{width, height}, directoryLister{directory} {}
    virtual ~LocalFile() {}

    virtual screens::Image retrieve();

   private:
    utils::DirectoryLister directoryLister;
};

}  // namespace imageretriever

#endif
