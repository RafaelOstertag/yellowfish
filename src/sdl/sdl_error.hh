#ifndef __SDL_ERROR_HH
#define __SDL_ERROR_HH

#include <stdexcept>

namespace sdl {
class SDLError final : public std::runtime_error {
    using std::runtime_error::runtime_error;
};
}  // namespace screens

#endif
