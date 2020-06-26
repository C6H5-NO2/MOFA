#pragma once
#include "Register.hpp"
#include <string_view>

namespace MOFA {
    class RegisterFile {
    public:
        RegisterFile();
        Register findReg(const std::string_view _name) const noexcept;

    private:
        Register gprs[32];
    };
}
