#include "memoryrwops.hh"

#include <cstdlib>
#include <sstream>

#include "sdl_error.hh"

using namespace sdl;

MemoryRWOps::MemoryRWOps(const void* ptr, size_t size)
    : buffer{std::make_unique<uint8_t[]>(size)} {
    memcpy(buffer.get(), ptr, size);

    memRWOps = SDL_RWFromConstMem(buffer.get(), static_cast<int>(size));
    if (memRWOps == nullptr) {
        std::ostringstream errorMessage;
        errorMessage << "Cannot create RWOps from memory: " << SDL_GetError();
        throw SDLError(errorMessage.str());
    }
}

MemoryRWOps::~MemoryRWOps() { destroyRWOps(); }

MemoryRWOps::MemoryRWOps(MemoryRWOps&& o) noexcept
    : buffer{std::move(o.buffer)}, memRWOps{o.memRWOps} {
    o.memRWOps = nullptr;
}

MemoryRWOps& MemoryRWOps::operator=(MemoryRWOps&& o) noexcept {
    destroyRWOps();

    buffer = std::move(o.buffer);

    memRWOps = o.memRWOps;
    o.memRWOps = nullptr;

    return *this;
}

void MemoryRWOps::destroyRWOps() {
    if (memRWOps != nullptr) {
        SDL_FreeRW(memRWOps);
        memRWOps = nullptr;
    }
}
