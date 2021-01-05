#ifndef __MEMORYRWOPS_HH
#define __MEMORYRWOPS_HH

#include <SDL.h>
#include <memory>

namespace sdl {

class MemoryRWOps final {
   public:
    MemoryRWOps(const void* ptr, size_t size);
    ~MemoryRWOps();

    MemoryRWOps(const MemoryRWOps&) = delete;
    MemoryRWOps& operator=(const MemoryRWOps&) = delete;

    MemoryRWOps(MemoryRWOps&& o) noexcept;
    MemoryRWOps& operator=(MemoryRWOps&& o) noexcept;

    constexpr SDL_RWops* getRWOps() const { return memRWOps; }

   private:
    std::unique_ptr<uint8_t[]> buffer{};
    SDL_RWops* memRWOps{nullptr};

    void destroyRWOps();
};

}  // namespace sdl

#endif
