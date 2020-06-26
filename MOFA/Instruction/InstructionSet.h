#pragma once
#include "Instruction.h"
#include <string_view>

namespace  MOFA {
    class InstructionSet {
    public:
        InstructionSet();
        ~InstructionSet() { puts("instr dtor"); }
        Instruction findInstr(const std::string_view _name) const noexcept;
    };
}
