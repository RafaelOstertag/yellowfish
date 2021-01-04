#ifndef __SCREEN_ERROR_HH
#define __SCREEN_ERROR_HH

#include <stdexcept>

namespace screens {
class ScreenError final : public std::runtime_error {
    using std::runtime_error::runtime_error;
};
}  // namespace screens

#endif
