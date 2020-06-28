#pragma once
#include <cstdint>

namespace MOFA {
    enum class InstructionFormat {
        R_RD_RS_RT,
        R_RD_RT_SHAMT,
        R_RD_RT_RS,
        R_RS,
        I_RT_RS_IMM16S,
        I_RT_RS_IMM16U,
        I_RT_IMM16U,
        I_RT_IMM16S_RS,
        I_RS_RT_LABEL,
        I_RS_LABEL,
        J_LABEL
    };


    struct Instruction {
        Instruction(const char* _name,
                    const std::uint8_t _opcode,
                    const InstructionFormat _format)
            : name(_name),
              opcode(_opcode),
              format(_format) {}

        const char* name;

        /// func if R-type
        const std::uint8_t opcode;

        const InstructionFormat format;
    };
}
