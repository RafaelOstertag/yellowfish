#include "memoryrwops.hh"

#include <sstream>

using namespace sdl;

MemoryRWOps::MemoryRWOps(const void* ptr, size_t size) : memRWOps{nullptr} {
    memRWOps = SDL_RWFromConstMem(ptr, size);
    if (memRWOps == nullptr) {
        std::ostringstream errorMessage;
        errorMessage << "Cannot create RWOps from memory: " << SDL_GetError();
        throw std::runtime_error(errorMessage.str());
    }
}

MemoryRWOps::~MemoryRWOps() { destroyRWOps(); }

MemoryRWOps::MemoryRWOps(MemoryRWOps&& o) : memRWOps{o.memRWOps} {
    o.memRWOps = nullptr;
}

MemoryRWOps& MemoryRWOps::operator=(MemoryRWOps&& o) {
    destroyRWOps();

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
