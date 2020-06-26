#pragma once
#include <memory>

namespace MOFA {
    class InstructionSet;
    class RegisterFile;

    namespace Global {
        const std::unique_ptr<InstructionSet>& getInstructionSet();
        const std::unique_ptr<RegisterFile>& getRegisterFile();
    }
}
