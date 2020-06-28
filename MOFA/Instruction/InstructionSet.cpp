#include "InstructionSet.h"

namespace MOFA {
    InstructionSet::InstructionSet() : instructions{
        Instruction("add", 0x20, InstructionFormat::R_RD_RS_RT),
        Instruction("addu", 0x21, InstructionFormat::R_RD_RS_RT),
        Instruction("sub", 0x22, InstructionFormat::R_RD_RS_RT),
        Instruction("subu", 0x23, InstructionFormat::R_RD_RS_RT),
        Instruction("and", 0x24, InstructionFormat::R_RD_RS_RT),
        Instruction("or", 0x25, InstructionFormat::R_RD_RS_RT),
        Instruction("xor", 0x26, InstructionFormat::R_RD_RS_RT),
        Instruction("nor", 0x27, InstructionFormat::R_RD_RS_RT),
        Instruction("slt", 0x2a, InstructionFormat::R_RD_RS_RT),
        Instruction("sltu", 0x2b, InstructionFormat::R_RD_RS_RT),
        Instruction("sll", 0x0, InstructionFormat::R_RD_RT_SHAMT),
        Instruction("srl", 0x2, InstructionFormat::R_RD_RT_SHAMT),
        Instruction("sra", 0x3, InstructionFormat::R_RD_RT_SHAMT),
        Instruction("sllv", 0x4, InstructionFormat::R_RD_RT_RS),
        Instruction("srlv", 0x6, InstructionFormat::R_RD_RT_RS),
        Instruction("srav", 0x7, InstructionFormat::R_RD_RT_RS),
        Instruction("jr", 0x8, InstructionFormat::R_RS),
        Instruction("addi", 0x8, InstructionFormat::I_RT_RS_IMM16),
        Instruction("addiu", 0x9, InstructionFormat::I_RT_RS_IMM16),
        Instruction("andi", 0xc, InstructionFormat::I_RT_RS_IMM16U),
        Instruction("ori", 0xd, InstructionFormat::I_RT_RS_IMM16U),
        Instruction("xori", 0xe, InstructionFormat::I_RT_RS_IMM16U),
        Instruction("sltiu", 0xb, InstructionFormat::I_RT_RS_IMM16),
        Instruction("lui", 0xf, InstructionFormat::I_RT_IMM16U),
        Instruction("lw", 0x23, InstructionFormat::I_RT_IMM16_RS),
        Instruction("sw", 0x2b, InstructionFormat::I_RT_IMM16_RS),
        Instruction("beq", 0x4, InstructionFormat::I_RS_RT_LABEL),
        Instruction("bne", 0x5, InstructionFormat::I_RS_RT_LABEL),
        Instruction("bgtz", 0x7, InstructionFormat::I_RS_LABEL),
        Instruction("j", 0x2, InstructionFormat::J_LABEL),
        Instruction("jal", 0x3, InstructionFormat::J_LABEL),
    } {}


    Instruction InstructionSet::findInstruction(const std::string_view _name) const noexcept {
        for(const auto& instr : instructions)
            if(instr.name == _name)
                return instr;
        return Instruction(nullptr, 0, InstructionFormat(0));
    }
}
