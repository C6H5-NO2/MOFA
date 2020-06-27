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
        const auto& getErrorList() const noexcept { return errorlist; }

    private:
        const TokenList tokenlist;
        std::map<std::string_view, unsigned> labelAddress;
        AddOnlyVector<std::uint32_t> machinecode;
        ErrorList errorlist;

        /// @brief In pass1, the address of labels are calculated.
        /// Generate error message if
        /// - Line not starts with comment/instruction/label
        /// - Same label appears on different lines
        void assemblePass1();

        /// @brief In pass2, instructions are assembled into machine code.
        /// @note Disgusting brute-force implementation.
        /// @todo
        void assemblePass2();
    };
}
