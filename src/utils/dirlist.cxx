#include "dirlist.hh"

#include <dirent.h>
#include <sys/types.h>

#include <cerrno>
#include <cstring>
#include <stdexcept>

using namespace utils;

namespace {
class DirectoryHandle {
   public:
    explicit DirectoryHandle(const char* path) {
        dir = opendir(path);
        if (dir == nullptr) {
            throw std::invalid_argument(std::strerror(errno));
        }
    }
    DirectoryHandle(const DirectoryHandle&) = delete;
    DirectoryHandle& operator=(const DirectoryHandle&) = delete;
    DirectoryHandle(DirectoryHandle&&) = delete;
    DirectoryHandle& operator=(DirectoryHandle&&) = delete;

    ~DirectoryHandle() {
        if (dir != nullptr) {
            closedir(dir);
            dir = nullptr;
        }
    }

    DIR* get() { return dir; }

   private:
    DIR* dir{nullptr};
};
}  // namespace

DirectoryLister::DirectoryLister(const std::string& path) : path{path} {}

std::vector<std::string> DirectoryLister::list() const {
    DirectoryHandle directoryHandle(path.c_str());

    std::vector<std::string> list;

    for (dirent* dent = readdir(directoryHandle.get()); dent != nullptr;
         dent = readdir(directoryHandle.get())) {
        if ((dent->d_type & DT_REG) != DT_REG) {
            continue;
        }

        list.push_back(path + "/" + dent->d_name);
    }

    return list;
}
