#ifndef __MEMORYRWOPS_HH
#define __MEMORYRWOPS_HH

#include <SDL.h>

namespace sdl {

class MemoryRWOps {
   public:
    MemoryRWOps(const void* ptr, size_t size);
    ~MemoryRWOps();

    MemoryRWOps(const MemoryRWOps&) = delete;
    MemoryRWOps& operator=(const MemoryRWOps&) = delete;

    MemoryRWOps(MemoryRWOps&& o);
    MemoryRWOps& operator=(MemoryRWOps&& o);

    constexpr SDL_RWops* getRWOps() const { return memRWOps; }

   private:
    uint8_t* buffer;
    SDL_RWops* memRWOps;

    void destroyRWOps();
};

}  // namespace sdl

#endif
