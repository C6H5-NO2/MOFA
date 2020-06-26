#pragma once
#include <vector>

namespace MOFA {
    template<typename _T>
    class AddOnlyVector {
    public:
        //AddOnlyVector(const unsigned _capacity = 0) : vec(_capacity) {}

        template<typename... _V>
        decltype(auto) add(_V&&... _v) { return vec.emplace_back(std::forward<_V>(_v)...); }

        unsigned length() const noexcept { return vec.size(); }

        bool empty() const noexcept { return vec.empty(); }

        const auto& operator[](const unsigned _pos) const { return vec[_pos]; }


        //auto cbegin() const noexcept { return vec.cbegin(); }

        //auto cend() const noexcept { return vec.cend(); }

    protected:
        std::vector<_T> vec;
    };
}
