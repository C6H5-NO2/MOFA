#pragma once
#include <string>

namespace MOFA {
    struct Register {
        Register(const char* _name,
                 const uint8_t _no) : name(_name),
                                      no(_no) {}

        const char* name;
        const uint8_t no;
    };
}
