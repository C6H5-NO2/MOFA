#include "Global.h"
#include "../Instruction/InstructionSet.h"
#include "../Register/RegisterFile.h"

namespace MOFA {
    std::unique_ptr<const InstructionSet> Global::instrset(new InstructionSet);
    std::unique_ptr<const RegisterFile> Global::regfile(new RegisterFile);

    const InstructionSet& Global::getInstructionSet() noexcept {
        if(!instrset)
            instrset = std::make_unique<const InstructionSet>();
        return *instrset;
    }

    const RegisterFile& Global::getRegisterFile() noexcept {
        if(!regfile)
            regfile = std::make_unique<const RegisterFile>();
        return *regfile;
    }
}
