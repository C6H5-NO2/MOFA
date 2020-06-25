#include "RegFile.h"

namespace MOFA {
    RegFile::RegFile() : gprs{
        Register("$zero", 0, RegisterType::GPR),
        Register("$at", 1, RegisterType::GPR),
        Register("$v0", 2, RegisterType::GPR),
        Register("$v1", 3, RegisterType::GPR),
        Register("$a0", 4, RegisterType::GPR),
        Register("$a1", 5, RegisterType::GPR),
        Register("$a2", 6, RegisterType::GPR),
        Register("$a3", 7, RegisterType::GPR),
        Register("$t0", 8, RegisterType::GPR),
        Register("$t1", 9, RegisterType::GPR),
        Register("$t2", 10, RegisterType::GPR),
        Register("$t3", 11, RegisterType::GPR),
        Register("$t4", 12, RegisterType::GPR),
        Register("$t5", 13, RegisterType::GPR),
        Register("$t6", 14, RegisterType::GPR),
        Register("$t7", 15, RegisterType::GPR),
        Register("$s0", 16, RegisterType::GPR),
        Register("$s1", 17, RegisterType::GPR),
        Register("$s2", 18, RegisterType::GPR),
        Register("$s3", 19, RegisterType::GPR),
        Register("$s4", 20, RegisterType::GPR),
        Register("$s5", 21, RegisterType::GPR),
        Register("$s6", 22, RegisterType::GPR),
        Register("$s7", 23, RegisterType::GPR),
        Register("$t8", 24, RegisterType::GPR),
        Register("$t9", 25, RegisterType::GPR),
        Register("$k0", 26, RegisterType::GPR),
        Register("$k1", 27, RegisterType::GPR),
        Register("$gp", 28, RegisterType::GPR),
        Register("$sp", 29, RegisterType::GPR),
        Register("$fp", 30, RegisterType::GPR),
        Register("$ra", 31, RegisterType::GPR)
    } {}


    namespace {
        uint8_t strToUint5Strict(const std::string_view _str) noexcept {
            const auto len = _str.length();
            if(len == 0 || len > 2)
                return -1;
            auto ten = _str[0], unit = _str.back();
            if(!isdigit(ten) || !isdigit(unit))
                return -1;

            ten -= '0';
            unit -= '0';
            if(len == 1)
                return unit;

            const auto num = ten * 10 + unit;
            if(num < 10 || num > 31)
                return -1;
            return num;
        }
    }


    Register RegFile::findReg(const std::string_view _name) const noexcept {
        const auto len = _name.length();
        if(len < 2 || len > 0 && _name[0] != '$')
            return Register("", -1, RegisterType::ERROR);

        // match patten $w0-$w31
        if(len > 2 && _name[1] == 'w')
            if(auto n = strToUint5Strict(_name.substr(2)); n < 32)
                return Register(std::string(_name), n, RegisterType::VR);

        // match patten $0-$31
        if(auto n = strToUint5Strict(_name.substr(1)); n < 32)
            return Register(std::string(_name), n, RegisterType::GPR);

        // match name of GPRs
        for(const auto& gpr : gprs)
            if(gpr.name == _name)
                return gpr;

        return Register("", -1, RegisterType::ERROR);
    }
}
