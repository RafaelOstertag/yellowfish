#ifndef __RETRIEVER_ERROR_HH
#define __RETRIEVER_ERROR_HH

#include <stdexcept>

namespace utils {
class RetrieverError final : public std::runtime_error {
    using std::runtime_error::runtime_error;
};
}  // namespace utils

#endif
