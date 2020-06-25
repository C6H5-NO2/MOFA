#pragma once
#include "Register.h"
#include <string_view>

namespace MOFA {
    class RegFile {
    public:
        RegFile();
        Register findReg(const std::string_view _name) const noexcept;

    private:
        Register gprs[32];
    };
}
