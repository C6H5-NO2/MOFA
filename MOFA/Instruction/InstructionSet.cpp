#include "InstructionSet.h"

namespace MOFA {
    InstructionSet::InstructionSet() : instructions{
        Instruction("add"),
        Instruction("addu"),
        Instruction("sub"),
        Instruction("subu"),
        Instruction("and"),
        Instruction("or"),
        Instruction("xor"),
        Instruction("nor"),
        Instruction("slt"),
        Instruction("sltu"),
        Instruction("sll"),
        Instruction("srl"),
        Instruction("sra"),
        Instruction("sllv"),
        Instruction("srlv"),
        Instruction("srav"),
        Instruction("jr"),
        Instruction("addi"),
        Instruction("addiu"),
        Instruction("andi"),
        Instruction("ori"),
        Instruction("xori"),
        Instruction("sltiu"),
        Instruction("lui"),
        Instruction("lw"),
        Instruction("sw"),
        Instruction("beq"),
        Instruction("bne"),
        Instruction("bgtz"),
        Instruction("j"),
        Instruction("jal")
    } {}

    Instruction InstructionSet::findInstruction(const std::string_view _name) const noexcept {
        for(const auto& instr : instructions)
            if(instr.name == _name)
                return instr;
        return Instruction(nullptr);
    }
}
