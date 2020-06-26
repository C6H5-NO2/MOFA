#pragma once
#include <string>

namespace MOFA {
    enum class RegisterType {
        GPR, ///< general purpose register
        VR, ///< vector register
        ERROR ///< error flag
    };


    struct Register {
        Register(std::string _name,
                 const uint8_t _num,
                 const RegisterType _type) : name(std::move(_name)),
                                             num(_num),
                                             type(_type) {}

        const std::string name;
        const uint8_t num;
        const RegisterType type;
    };
}
