#pragma once
#include <string>

namespace MOFA {
    // todo
    struct Instruction {
        Instruction(const char* _name) : name(_name) {}
        const char* name;
    };
}
