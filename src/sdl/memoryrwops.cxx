#include "memoryrwops.hh"

#include <cstdlib>
#include <sstream>

using namespace sdl;

MemoryRWOps::MemoryRWOps(const void* ptr, size_t size)
    : buffer{nullptr}, memRWOps{nullptr} {
    buffer = new uint8_t[size];
    memcpy(buffer, ptr, size);

    memRWOps = SDL_RWFromConstMem(buffer, size);
    if (memRWOps == nullptr) {
        std::ostringstream errorMessage;
        errorMessage << "Cannot create RWOps from memory: " << SDL_GetError();
        throw std::runtime_error(errorMessage.str());
    }
}

MemoryRWOps::~MemoryRWOps() { destroyRWOps(); }

MemoryRWOps::MemoryRWOps(MemoryRWOps&& o)
    : buffer{o.buffer}, memRWOps{o.memRWOps} {
    o.buffer = nullptr;
    o.memRWOps = nullptr;
}

MemoryRWOps& MemoryRWOps::operator=(MemoryRWOps&& o) {
    destroyRWOps();

    buffer = o.buffer;
    o.buffer = nullptr;

    memRWOps = o.memRWOps;
    o.memRWOps = nullptr;

    return *this;
}

void MemoryRWOps::destroyRWOps() {
    if (buffer != nullptr) {
        delete[] buffer;
        buffer = nullptr;
    }

    if (memRWOps != nullptr) {
        SDL_FreeRW(memRWOps);
        memRWOps = nullptr;
    }
}
