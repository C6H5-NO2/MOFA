#pragma once
#include <string_view>

namespace MOFA {
    /// Strict convert string to int. The expected int should be zero or positive.
    inline int strToIntStrict(const std::string_view _str) noexcept {
        if(_str.empty() || !isdigit(_str[0]))
            return -1;
        return 0;
    }
}
