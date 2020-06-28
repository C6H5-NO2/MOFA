#include "Assembler.h"
#include "../Tokenizer/Token.h"
#include "../Instruction/InstructionSet.h"
#include "../Register/RegisterFile.h"
#include "../Utility/ErrorMessage.hpp"
#include "../Utility/Global.h"

namespace MOFA {
    Assembler::Assembler(TokenList _tokenlist) : tokenlist(std::move(_tokenlist)) {}


    bool Assembler::assemble() {
        labelAddress.clear();
        machinecode.clear();
        errorlist.clear();
        assemblePass1();
        if(!errorlist.empty())
            return false;
        assemblePass2();
        return errorlist.empty();
    }


    const ErrorList& Assembler::getErrorList() const noexcept {
        return errorlist;
    }


    void Assembler::assemblePass1() {
        std::uint32_t pc = 0u;
        for(auto& tokenline : tokenlist) {
            const auto len = tokenline.length();
            for(auto idx = 0u; idx < len; ++idx) {
                auto& token = tokenline[idx];
                if(token.type == TokenType::COMMENT)
                    break;
                if(token.type == TokenType::INSTRUCTION) {
                    pc += 4;
                    break;
                }
                if(token.type == TokenType::LABEL) {
                    if(len < idx + 2 || tokenline[idx + 1].type != TokenType::COLON) {
                        errorlist.add(token.line, ErrorType::INVALID_INSTRUCTION, token.token);
                        break;
                    }
                    ++idx; // skip colon
                    auto [_, success] = labelAddress.emplace(token.token, pc);
                    if(!success) {
                        errorlist.add(token.line, ErrorType::ALREADY_DEFINED, token.token);
                        break;
                    }
                }
                else {
                    errorlist.add(token.line, ErrorType::INVALID_INSTRUCTION, token.token);
                    break;
                }
            }
        }
    }


    void Assembler::assemblePass2() {
        std::uint32_t pc = 0u;
        for(auto& tokenline : tokenlist) {
            const auto len = tokenline.length();
            for(auto idx = 0u; idx < len; ++idx) {
                auto& token = tokenline[idx];
                if(token.type == TokenType::COMMENT)
                    break;
                if(token.type == TokenType::INSTRUCTION) {
                    pc += 4;
                    const auto instr = Global::getInstructionSet().findInstruction(token.token);
                    const auto [code, delta] = [&] {
                        switch(instr.format) {
                            case InstructionFormat::R_RD_RS_RT:
                                return parseFormat0(tokenline, idx, instr.opcode);
                            case InstructionFormat::R_RD_RT_SHAMT:
                                return parseFormat1(tokenline, idx, instr.opcode);
                            case InstructionFormat::R_RD_RT_RS:
                                return parseFormat2(tokenline, idx, instr.opcode);
                            case InstructionFormat::R_RS:
                                return parseFormat3(tokenline, idx, instr.opcode);
                            case InstructionFormat::I_RT_RS_IMM16S:
                                return parseFormat4(tokenline, idx, instr.opcode);
                            case InstructionFormat::I_RT_RS_IMM16U:
                                return parseFormat5(tokenline, idx, instr.opcode);
                            case InstructionFormat::I_RT_IMM16U:
                                return parseFormat6(tokenline, idx, instr.opcode);
                            case InstructionFormat::I_RT_IMM16S_RS:
                                return parseFormat7(tokenline, idx, instr.opcode);
                            case InstructionFormat::I_RS_RT_LABEL:
                                return parseFormat8(tokenline, idx, instr.opcode, pc);
                            case InstructionFormat::I_RS_LABEL:
                                return parseFormat9(tokenline, idx, instr.opcode, pc);
                            case InstructionFormat::J_LABEL:
                                return parseFormatA(tokenline, idx, instr.opcode);
                            default:
                                return ParseResult(0, 0);
                        }
                    }();
                    idx += delta;
                    // if any error in parsing, delta is 0
                    // error if next token (if any) is not comment
                    if(delta && (idx == len || tokenline[idx].type == TokenType::COMMENT))
                        machinecode.add(code);
                    else
                        errorlist.add(token.line, ErrorType::INVALID_OPERAND, "after " + token.token);
                    break;
                }
                if(token.type == TokenType::LABEL)
                    ++idx; // skip colon
                else {
                    errorlist.add(token.line, ErrorType::INVALID_OPERAND, token.token);
                    break;
                }
            }
        }
    }


    namespace {
        constexpr std::uint32_t parseTypeR(const std::uint32_t _rs,
                                           const std::uint32_t _rt,
                                           const std::uint32_t _rd,
                                           const std::uint32_t _shamt,
                                           const std::uint32_t _func) {
            return (_rs & 0x1f) << 21
                   | (_rt & 0x1f) << 16
                   | (_rd & 0x1f) << 11
                   | (_shamt & 0x1f) << 6
                   | (_func & 0x3f);
        }

        constexpr std::uint32_t parseTypeI(const std::uint32_t _opcode,
                                           const std::uint32_t _rs,
                                           const std::uint32_t _rt,
                                           const std::uint32_t _imm) {
            return _opcode << 26
                   | (_rs & 0x1f) << 21
                   | (_rt & 0x1f) << 16
                   | (_imm & 0xffff);
        }

        constexpr std::uint32_t parseTypeJ(const std::uint32_t _opcode,
                                           const std::uint32_t _addr) {
            return _opcode << 26
                   | (_addr & 0x3ffffff);
        }

        bool isSignedInt16(const Token& _token) {
            switch(_token.type) {
                case TokenType::IMM5:
                    return true;
                case TokenType::IMM16:
                    return std::strtoll(_token.token.c_str(), nullptr, 0) <= INT16_MAX;
                default:
                    return false;
            }
        }

        bool isUnsignedint16(const Token& _token) {
            switch(_token.type) {
                case TokenType::IMM5:
                    return true;
                case TokenType::IMM16:
                    return std::strtoll(_token.token.c_str(), nullptr, 0) >= 0;
                default:
                    return false;
            }
        }
    }


    Assembler::ParseResult Assembler::parseFormat0(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _func) const {
        const auto len = _tokenline.length();
        if(len < _idx + 4)
            return std::make_pair(0, 0);

        const auto &t1 = _tokenline[_idx + 1], &t2 = _tokenline[_idx + 2], &t3 = _tokenline[_idx + 3];
        if(t1.type == TokenType::GEN_PUR_REG
           && t2.type == TokenType::GEN_PUR_REG
           && t3.type == TokenType::GEN_PUR_REG) {
            const auto rd = Global::getRegisterFile().findRegister(t1.token).no,
                       rs = Global::getRegisterFile().findRegister(t2.token).no,
                       rt = Global::getRegisterFile().findRegister(t3.token).no;
            const auto code = parseTypeR(rs, rt, rd, 0, _func);
            return std::make_pair(code, 4);
        }

        return std::make_pair(0, 0);
    }


    Assembler::ParseResult Assembler::parseFormat1(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _func) const {
        const auto len = _tokenline.length();
        if(len < _idx + 4)
            return std::make_pair(0, 0);

        const auto &t1 = _tokenline[_idx + 1], &t2 = _tokenline[_idx + 2], &t3 = _tokenline[_idx + 3];
        if(t1.type == TokenType::GEN_PUR_REG
           && t2.type == TokenType::GEN_PUR_REG
           && t3.type == TokenType::IMM5) {
            const auto rd = Global::getRegisterFile().findRegister(t1.token).no,
                       rt = Global::getRegisterFile().findRegister(t2.token).no;
            const auto shamt = std::strtoll(t3.token.c_str(), nullptr, 0);
            const auto code = parseTypeR(0, rt, rd, shamt, _func);
            return std::make_pair(code, 4);
        }

        return std::make_pair(0, 0);
    }


    Assembler::ParseResult Assembler::parseFormat2(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _func) const {
        const auto len = _tokenline.length();
        if(len < _idx + 4)
            return std::make_pair(0, 0);

        const auto &t1 = _tokenline[_idx + 1], &t2 = _tokenline[_idx + 2], &t3 = _tokenline[_idx + 3];
        if(t1.type == TokenType::GEN_PUR_REG
           && t2.type == TokenType::GEN_PUR_REG
           && t3.type == TokenType::GEN_PUR_REG) {
            const auto rd = Global::getRegisterFile().findRegister(t1.token).no,
                       rt = Global::getRegisterFile().findRegister(t2.token).no,
                       rs = Global::getRegisterFile().findRegister(t3.token).no;
            const auto code = parseTypeR(rs, rt, rd, 0, _func);
            return std::make_pair(code, 4);
        }

        return std::make_pair(0, 0);
    }


    Assembler::ParseResult Assembler::parseFormat3(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _func) const {
        const auto len = _tokenline.length();
        if(len < _idx + 2)
            return std::make_pair(0, 0);

        const auto& t1 = _tokenline[_idx + 1];
        if(t1.type == TokenType::GEN_PUR_REG) {
            const auto rs = Global::getRegisterFile().findRegister(t1.token).no;
            const auto code = parseTypeR(rs, 0, 0, 0, _func);
            return std::make_pair(code, 2);
        }

        return std::make_pair(0, 0);
    }


    Assembler::ParseResult Assembler::parseFormat4(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _opcode) const {
        const auto len = _tokenline.length();
        if(len < _idx + 4)
            return std::make_pair(0, 0);

        const auto &t1 = _tokenline[_idx + 1], &t2 = _tokenline[_idx + 2], &t3 = _tokenline[_idx + 3];
        if(t1.type == TokenType::GEN_PUR_REG
           && t2.type == TokenType::GEN_PUR_REG
           && isSignedInt16(t3)) {
            const auto rt = Global::getRegisterFile().findRegister(t1.token).no,
                       rs = Global::getRegisterFile().findRegister(t2.token).no;
            const auto imm = std::strtoll(t3.token.c_str(), nullptr, 0);
            const auto code = parseTypeI(_opcode, rs, rt, imm);
            return std::make_pair(code, 4);
        }

        return std::make_pair(0, 0);
    }


    Assembler::ParseResult Assembler::parseFormat5(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _opcode) const {
        const auto len = _tokenline.length();
        if(len < _idx + 4)
            return std::make_pair(0, 0);

        const auto &t1 = _tokenline[_idx + 1], &t2 = _tokenline[_idx + 2], &t3 = _tokenline[_idx + 3];
        if(t1.type == TokenType::GEN_PUR_REG
           && t2.type == TokenType::GEN_PUR_REG
           && isUnsignedint16(t3)) {
            const auto rt = Global::getRegisterFile().findRegister(t1.token).no,
                       rs = Global::getRegisterFile().findRegister(t2.token).no;
            const auto imm = std::strtoll(t3.token.c_str(), nullptr, 0);
            const auto code = parseTypeI(_opcode, rs, rt, imm);
            return std::make_pair(code, 4);
        }

        return std::make_pair(0, 0);
    }


    Assembler::ParseResult Assembler::parseFormat6(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _opcode) const {
        const auto len = _tokenline.length();
        if(len < _idx + 3)
            return std::make_pair(0, 0);

        const auto &t1 = _tokenline[_idx + 1], &t2 = _tokenline[_idx + 2];
        if(t1.type == TokenType::GEN_PUR_REG
           && isUnsignedint16(t2)) {
            const auto rt = Global::getRegisterFile().findRegister(t1.token).no;
            const auto imm = std::strtoll(t2.token.c_str(), nullptr, 0);
            const auto code = parseTypeI(_opcode, 0, rt, imm);
            return std::make_pair(code, 3);
        }

        return std::make_pair(0, 0);
    }


    Assembler::ParseResult Assembler::parseFormat7(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _opcode) const {
        const auto len = _tokenline.length();
        if(len < _idx + 6)
            return std::make_pair(0, 0);

        const auto &t1 = _tokenline[_idx + 1],
                   &t2 = _tokenline[_idx + 2],
                   &t3 = _tokenline[_idx + 3],
                   &t4 = _tokenline[_idx + 4],
                   &t5 = _tokenline[_idx + 5];
        if(t1.type == TokenType::GEN_PUR_REG
           && isSignedInt16(t2)
           && t3.type == TokenType::PAREN_L
           && t4.type == TokenType::GEN_PUR_REG
           && t5.type == TokenType::PAREN_R) {
            const auto rt = Global::getRegisterFile().findRegister(t1.token).no,
                       rs = Global::getRegisterFile().findRegister(t4.token).no;
            const auto imm = std::strtoll(t2.token.c_str(), nullptr, 0);
            const auto code = parseTypeI(_opcode, rs, rt, imm);
            return std::make_pair(code, 6);
        }

        return std::make_pair(0, 0);
    }


    Assembler::ParseResult Assembler::parseFormat8(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _opcode, const std::uint32_t _pc) const {
        const auto len = _tokenline.length();
        if(len < _idx + 4)
            return std::make_pair(0, 0);

        const auto &t1 = _tokenline[_idx + 1],
                   &t2 = _tokenline[_idx + 2],
                   &t3 = _tokenline[_idx + 3];
        if(t1.type == TokenType::GEN_PUR_REG
           && t2.type == TokenType::GEN_PUR_REG
           && t3.type == TokenType::LABEL) {
            auto iter = labelAddress.find(t3.token);
            if(iter == labelAddress.end())
                return std::make_pair(0, 0);

            const auto imm = int32_t(iter->second >> 2) - int32_t(_pc >> 2);
            const auto rs = Global::getRegisterFile().findRegister(t1.token).no,
                       rt = Global::getRegisterFile().findRegister(t2.token).no;
            const auto code = parseTypeI(_opcode, rs, rt, imm);
            return std::make_pair(code, 4);
        }

        return std::make_pair(0, 0);
    }


    Assembler::ParseResult Assembler::parseFormat9(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _opcode, const std::uint32_t _pc) const {
        const auto len = _tokenline.length();
        if(len < _idx + 3)
            return std::make_pair(0, 0);

        const auto &t1 = _tokenline[_idx + 1],
                   &t2 = _tokenline[_idx + 2];
        if(t1.type == TokenType::GEN_PUR_REG
           && t2.type == TokenType::LABEL) {
            auto iter = labelAddress.find(t2.token);
            if(iter == labelAddress.end())
                return std::make_pair(0, 0);

            const auto imm = int32_t(iter->second >> 2) - int32_t(_pc >> 2);
            const auto rs = Global::getRegisterFile().findRegister(t1.token).no;
            const auto code = parseTypeI(_opcode, rs, 0, imm);
            return std::make_pair(code, 3);
        }

        return std::make_pair(0, 0);
    }


    Assembler::ParseResult Assembler::parseFormatA(const TokenLine& _tokenline, const unsigned _idx, const std::uint8_t _opcode) const {
        const auto len = _tokenline.length();
        if(len < _idx + 2)
            return std::make_pair(0, 0);

        const auto& t1 = _tokenline[_idx + 1];
        if(t1.type == TokenType::LABEL) {
            auto iter = labelAddress.find(t1.token);
            if(iter == labelAddress.end())
                return std::make_pair(0, 0);

            const auto addr = iter->second >> 2;
            const auto code = parseTypeJ(_opcode, addr);
            return std::make_pair(code, 2);
        }

        return std::make_pair(0, 0);
    }
}
