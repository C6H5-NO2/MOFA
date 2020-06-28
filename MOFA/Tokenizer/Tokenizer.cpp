#include "Tokenizer.h"
#include "Token.h"
#include "../Utility/StringViewBuffer.hpp"
#include "../Utility/ErrorMessage.hpp"

namespace MOFA {
    Tokenizer::Tokenizer(SourceCode _source) : source(std::move(_source)) {}


    bool Tokenizer::tokenize() {
        tokenlist.clear();
        errorlist.clear();
        for(auto& line : source) {
            auto tokenline = tokenizeLine(line);
            if(!tokenline.empty())
                tokenlist.add(std::move(tokenline));
        }
        return errorlist.empty();
    }


    const TokenList& Tokenizer::getTokenList() const noexcept { return tokenlist; }


    const ErrorList& Tokenizer::getErrorList() const noexcept { return errorlist; }


#define ADD_TOKEN(candidate) addToken(tokenline, candidate, line)
#define PUSH_BUFFER pushBuffer(tokenline, tokenbuffer, line)

    TokenLine Tokenizer::tokenizeLine(const SourceLine& _sourceline) {
        TokenLine tokenline;
        const std::string_view linecode(_sourceline.code);
        const auto line = _sourceline.line;
        const auto len = linecode.length();
        if(!len)
            return tokenline;

        StringViewBuffer tokenbuffer(linecode);

        for(auto column = 0u; column < len; ++column) {
            switch(linecode[column]) {
                case '#': // comment
                    PUSH_BUFFER;
                    ADD_TOKEN(linecode.substr(column));
                    column = len;
                    break;

                case ' ':
                case '\t':
                case ',': // delimiter
                    PUSH_BUFFER;
                    break;

                case ':':
                case '(':
                case ')': // mark
                    PUSH_BUFFER;
                    ADD_TOKEN(linecode.substr(column, 1));
                    break;

                case '$': // prospective register
                    PUSH_BUFFER;
                    tokenbuffer.append(column);
                    break;


                case '\'': // prospective char literal
                    PUSH_BUFFER;
                    {
                        auto escape = true;
                        do {
                            const auto preview = linecode[column];
                            tokenbuffer.append(column);
                            if(escape)
                                escape = false;
                            else {
                                if(preview == '\\')
                                    escape = true;
                                else if(preview == '\'') {
                                    PUSH_BUFFER;
                                    break;
                                }
                            }
                            ++column;
                        } while(column < len);
                    }
                    break;

                default:
                    tokenbuffer.append(column);
                    break;
            }
        }

        PUSH_BUFFER;

        return tokenline;
    }


    void Tokenizer::addToken(TokenLine& _dst, const std::string_view& _candidate, const unsigned _line) {
        if(_candidate.empty())
            return;
        const auto& token = _dst.add(_candidate, _line);
        if(token.type == TokenType::ERROR) {
            errorlist.add(_line, ErrorType::INVALID_TOKEN, token.token);
            return;
        }

        // todo: delete these after support
        if(token.type == TokenType::UNSUPPORTED_CHAR_CANDIDATE)
            errorlist.add(_line, ErrorType::UNSUPPORTED_FEATURE, "char candidate " + token.token);
        else if(token.type == TokenType::UNSUPPORTED_UINT32 || token.type == TokenType::UNSUPPORTED_INT32)
            errorlist.add(_line, ErrorType::UNSUPPORTED_FEATURE, "32-bit immediate " + token.token);
    }


    void Tokenizer::pushBuffer(TokenLine& _dst, StringViewBuffer& _buf, const unsigned _line) {
        addToken(_dst, _buf.toSV(), _line);
        _buf.softclear();
    }
}
