#pragma once
#include <vector>

namespace MOFA {
    template<typename _T>
    class AddOnlyVector {
    public:
        template<typename... _V>
        decltype(auto) add(_V&&... _v) { return vec.emplace_back(std::forward<_V>(_v)...); }

        void clear() noexcept { vec.clear(); }

        unsigned length() const noexcept { return vec.size(); }

        bool empty() const noexcept { return vec.empty(); }

        const auto& operator[](const unsigned _pos) const { return vec[_pos]; }

        auto begin() const noexcept { return vec.cbegin(); }

        auto end() const noexcept { return vec.cend(); }

    protected:
        std::vector<_T> vec;
    };
}
