#include "RegisterFile.h"

namespace MOFA {
    RegisterFile::RegisterFile() : registers{
        Register("$zero", 0),
        Register("$at", 1),
        Register("$v0", 2),
        Register("$v1", 3),
        Register("$a0", 4),
        Register("$a1", 5),
        Register("$a2", 6),
        Register("$a3", 7),
        Register("$t0", 8),
        Register("$t1", 9),
        Register("$t2", 10),
        Register("$t3", 11),
        Register("$t4", 12),
        Register("$t5", 13),
        Register("$t6", 14),
        Register("$t7", 15),
        Register("$s0", 16),
        Register("$s1", 17),
        Register("$s2", 18),
        Register("$s3", 19),
        Register("$s4", 20),
        Register("$s5", 21),
        Register("$s6", 22),
        Register("$s7", 23),
        Register("$t8", 24),
        Register("$t9", 25),
        Register("$k0", 26),
        Register("$k1", 27),
        Register("$gp", 28),
        Register("$sp", 29),
        Register("$fp", 30),
        Register("$ra", 31),
        Register("$0", 0),
        Register("$1", 1),
        Register("$2", 2),
        Register("$3", 3),
        Register("$4", 4),
        Register("$5", 5),
        Register("$6", 6),
        Register("$7", 7),
        Register("$8", 8),
        Register("$9", 9),
        Register("$10", 10),
        Register("$11", 11),
        Register("$12", 12),
        Register("$13", 13),
        Register("$14", 14),
        Register("$15", 15),
        Register("$16", 16),
        Register("$17", 17),
        Register("$18", 18),
        Register("$19", 19),
        Register("$20", 20),
        Register("$21", 21),
        Register("$22", 22),
        Register("$23", 23),
        Register("$24", 24),
        Register("$25", 25),
        Register("$26", 26),
        Register("$27", 27),
        Register("$28", 28),
        Register("$29", 29),
        Register("$30", 30),
        Register("$31", 31)
    } {}


    Register RegisterFile::findRegister(const std::string_view _name) const noexcept {
        for(const auto& reg : registers)
            if(reg.name == _name)
                return reg;
        return Register(nullptr, -1);
    }
}
