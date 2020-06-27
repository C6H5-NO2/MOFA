#include "Tokenizer.h"
#include "Token.h"
#include "../Utility/StringViewBuffer.hpp"
#include "../Utility/ErrorMessage.hpp"

namespace MOFA {
    Tokenizer::Tokenizer(SourceCode _source) : source(std::move(_source)) {}


    bool Tokenizer::tokenize() {
        for(auto& line : source) {
            auto tokenline = tokenizeLine(line);
            if(!tokenline.empty())
                tokenlist.add(std::move(tokenline));
        }
        return errorlist.empty();
    }


    namespace {
        /// @brief Add token and generate error message.
        void addToken(TokenLine& _dst, const std::string_view& _candidate, const unsigned _line, ErrorList& _errlst) {
            if(_candidate.empty())
                return;
            const auto& token = _dst.add(_candidate, _line);
            if(token.type == TokenType::ERROR)
                _errlst.add(_line, ErrorType::INVALID_TOKEN, token.token);
            else if(token.type == TokenType::UNSUPPORTED_CHAR_CANDIDATE)
                _errlst.add(_line, ErrorType::UNSUPPORTED_FEATURE, "char candidate " + token.token);
            else if(token.type == TokenType::UNSUPPORTED_UINT32 || token.type == TokenType::UNSUPPORTED_INT32)
                _errlst.add(_line, ErrorType::UNSUPPORTED_FEATURE, "32-bit immediate " + token.token);
        }

        /// @brief Add token constructed by buffer to line. buffer is cleared afterwards.
        void pushBuffer(TokenLine& _dst, StringViewBuffer& _buf, const unsigned _line, ErrorList& _errlst) {
            addToken(_dst, _buf.toSV(), _line, _errlst);
            _buf.softclear();
        }
    }


#define ADD_TOKEN(candidate) addToken(tokenline, candidate, line, errorlist)
#define PUSH_BUFFER pushBuffer(tokenline, tokenbuffer, line, errorlist)

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
}
