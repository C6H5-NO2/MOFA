#include "Assembler.h"
#include "../Tokenizer/Token.h"
#include "../Utility/ErrorMessage.hpp"

namespace MOFA {
    Assembler::Assembler(TokenList _tokenlist) : tokenlist(std::move(_tokenlist)) {}


    bool Assembler::assemble() {
        labelAddress.clear();
        machinecode.clear();
        errorlist.clear();

        assemblePass1();
        if(!errorlist.empty())
            return false;

        //assemblePass2();

        return errorlist.empty();
    }


    void Assembler::assemblePass1() {
        auto pc = 0u;
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
                        errorlist.add(token.line, ErrorType::INVALID_INSTURCTION, token.token);
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
                    errorlist.add(token.line, ErrorType::INVALID_INSTURCTION, token.token);
                    break;
                }
            }
        }
    }


    void Assembler::assemblePass2() {
        auto pc = 0u;
        for(auto& tokenline : tokenlist) {
            const auto len = tokenline.length();
            for(auto idx = 0u; idx < len; ++idx) {
                auto& token = tokenline[idx];
                if(token.type == TokenType::COMMENT)
                    break;
                if(token.type == TokenType::INSTRUCTION) {
                    pc += 4;
                    // todo: check validation and translate

                    // todo: error if next token (if any) is not comment

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
}
