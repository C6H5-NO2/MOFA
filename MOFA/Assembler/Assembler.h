#pragma once
#include "../Tokenizer/TokenView.hpp"
#include "../Utility/ErrorList.hpp"
#include <map>

namespace MOFA {
    class Assembler {
    public:
        Assembler(TokenList _tokenlist);
        bool assemble();
        const auto& getLabelAddress() const noexcept { return labelAddress; }
        const auto& getMachineCode() const noexcept { return machinecode; }
        const ErrorList& getErrorList() const noexcept;

    private:
        const TokenList tokenlist;
        /// 32-bit FULL address with least 2 bits being 0
        std::map<std::string_view, std::uint32_t> labelAddress;
        AddOnlyVector<std::uint32_t> machinecode;
        ErrorList errorlist;

        /// @brief In pass1, the address of labels are calculated.
        /// Generate error message if
        /// - Line not starts with comment/instruction/label
        /// - Same label appears on different lines
        void assemblePass1();

        /// @brief In pass2, instructions are assembled into machine code.
        /// @note Disgusting brute-force implementation.
        void assemblePass2();


        /// @todo move all parsers to a separate class
        /// @brief First element is the corresponding machine code.\n
        /// Second element is the delta of _idx after parsing the instruction.
        /// i.e. #operands + 1. Zero indicates an error occurs.
        using ParseResult = std::pair<std::uint32_t, unsigned>;

        /// @brief InstructionFormat::R_RD_RS_RT
        ParseResult parseFormat0(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _func) const;

        /// @brief InstructionFormat::R_RD_RT_SHAMT
        ParseResult parseFormat1(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _func) const;

        /// @brief InstructionFormat::R_RD_RT_RS
        ParseResult parseFormat2(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _func) const;

        /// @brief InstructionFormat::R_RS
        ParseResult parseFormat3(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _func) const;

        /// @brief InstructionFormat::I_RT_RS_IMM16S
        ParseResult parseFormat4(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _opcode) const;

        /// @brief InstructionFormat::I_RT_RS_IMM16U
        ParseResult parseFormat5(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _opcode) const;

        /// @brief InstructionFormat::I_RT_IMM16U
        ParseResult parseFormat6(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _opcode) const;

        /// @brief InstructionFormat::I_RT_IMM16S_RS
        ParseResult parseFormat7(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _opcode) const;

        /// @brief InstructionFormat::I_RS_RT_LABEL
        ParseResult parseFormat8(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _opcode, const std::uint32_t _pc) const;

        /// @brief InstructionFormat::I_RS_LABEL
        ParseResult parseFormat9(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _opcode, const std::uint32_t _pc) const;

        /// @brief InstructionFormat::J_LABEL
        ParseResult parseFormatA(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _opcode) const;
    };
}
