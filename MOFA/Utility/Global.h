#pragma once
#include <memory>

namespace MOFA {
    class InstructionSet;
    class RegisterFile;

    struct Global {
        static const InstructionSet& getInstructionSet() noexcept;
        static const RegisterFile& getRegisterFile() noexcept;

    private:
        static std::unique_ptr<const InstructionSet> instrset;
        static std::unique_ptr<const RegisterFile> regfile;
    };
}
