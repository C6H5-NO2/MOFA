#pragma once
#include "Instruction.hpp"
#include <string_view>

namespace MOFA {
    class InstructionSet {
    public:
        InstructionSet();
        Instruction findInstruction(const std::string_view _name) const noexcept;

    private:
        Instruction instructions[31];
    };
}
