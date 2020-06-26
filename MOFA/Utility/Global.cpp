#include "Global.h"
#include "../Instruction/InstructionSet.h"
#include "../Register/RegisterFile.h"

namespace MOFA::Global {
    const std::unique_ptr<InstructionSet>& getInstructionSet() {
        static std::unique_ptr<InstructionSet> instrset(new InstructionSet);
        return instrset;
    }

    const std::unique_ptr<RegisterFile>& getRegisterFile() {
        static std::unique_ptr<RegisterFile> regfile(new RegisterFile);
        return regfile;
    }
}
