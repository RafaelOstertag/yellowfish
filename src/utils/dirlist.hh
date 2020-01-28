#ifndef __DIRLIST_HH
#define __DIRLIST_HH

#include <string>
#include <vector>

namespace utils {
class DirectoryLister {
  public:
    DirectoryLister(const std::string& path);
    std::vector<std::string> list() const;

  private:
    std::string path;
};
} // namespace utils

#endif
