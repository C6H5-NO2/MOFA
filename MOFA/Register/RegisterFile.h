#pragma once
#include "Register.hpp"
#include <string_view>

namespace MOFA {
    class RegisterFile {
    public:
        RegisterFile();

        /// @return Register(nullptr, -1) if not found
        Register findRegister(const std::string_view _name) const noexcept;

    private:
        Register registers[64];
    };
}
