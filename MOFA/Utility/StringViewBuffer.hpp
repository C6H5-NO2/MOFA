#pragma once
#include <string_view>

namespace MOFA {
    class StringViewBuffer {
    public:
        StringViewBuffer(const std::string_view _base) : offset(0),
                                                         count(0),
                                                         base(_base) {}

        void append(const unsigned _col) noexcept {
            ++count;
            if(count == 1)
                offset = _col;
        }

        void softclear() noexcept {
            offset += count;
            count = 0;
        }

        std::string_view toSV() const noexcept {
            return base.substr(offset, count);
        }

    private:
        unsigned offset, count;
        std::string_view base;
    };
}
