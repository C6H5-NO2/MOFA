#include "Token.h"
#include "../Utility/Global.h"
#include "../Register/RegisterFile.h"
#include "../Instruction/InstructionSet.h"
#include <regex>

namespace MOFA {
    namespace {
        TokenType matchType(const std::string& _candidate) {
            auto len = _candidate.length();
            if(!len)
                return TokenType::ERROR; // Tokenizer Error
            const auto firstchr = _candidate[0];

            // is comment?
            if(firstchr == '#')
                return TokenType::COMMENT;

            // is symbol?
            if(len == 1)
                switch(firstchr) {
                        //case '+': return TokenType::POSITIVE;
                        //case '-': return TokenType::NEGATIVE;
                    case ':': return TokenType::COLON;
                    case '(': return TokenType::PAREN_L;
                    case ')': return TokenType::PAREN_R;
                    case '[': return TokenType::BRACKET_L;
                    case ']': return TokenType::BRACKET_R;
                    default: break;
                }

            // todo: process char literals
            if(firstchr == '\'') {
                return TokenType::ERROR;
            }

            // is register?
            if(firstchr == '$') {
                auto reg = Global::getRegisterFile()->findReg(_candidate);
                switch(reg.type) {
                    case RegisterType::GPR: return TokenType::GEN_PUR_REG;
                    case RegisterType::VR: return TokenType::VEC_REG;
                    default: return TokenType::ERROR;
                }
            }

            // is integer?
            errno = 0;
            char* endptr = nullptr;
            const auto llval = std::strtoll(_candidate.data(), &endptr, 0);
            // if there is a match
            if(_candidate.data() < endptr) {
                // if it is a full valid match
                if(!errno && _candidate.data() + _candidate.length() == endptr) {
                    if(llval >= 0) {
                        if(llval <= 31)
                            return TokenType::UINT5;
                        if(llval <= UINT16_MAX)
                            return TokenType::UINT16;
                        if(llval <= UINT32_MAX)
                            return TokenType::UINT32;
                        return TokenType::ERROR;
                    }
                    if(llval >= INT16_MIN)
                        return TokenType::INT16;
                    if(llval >= INT32_MIN)
                        return TokenType::INT32;
                }
                return TokenType::ERROR;
            }

            // is instruction?
            //auto instr = Global::instrset->findInstr(_candidate);
            // todo: check instr

            // is identifier?
            std::cmatch ma;
            std::regex re("^[_a-zA-Z][_a-zA-Z0-9]*");
            if(std::regex_match(_candidate.data(), ma, re))
                return TokenType::IDENTIFIER;

            return TokenType::ERROR;
        }
    }


    Token::Token(const std::string_view _token) : token(_token),
                                                  type(matchType(token)) {}
}
