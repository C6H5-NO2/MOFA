#include "Tokenizer.h"
#include "Token.h"
#include "../Utility/StringViewBuffer.hpp"
#include "../Utility/ErrorMessage.hpp"
#include "../SourceCode/SourceLine.hpp"

namespace MOFA {
    Tokenizer::Tokenizer(SourceCode _source) : source(std::move(_source)) {}


    bool Tokenizer::tokenize() {
        for(auto i = 0u; i < source.length(); ++i)
            allTokens.add(tokenizeLine(source[i]));
        return errorlist.empty();
    }


    namespace {
        /// @brief Add token and generate error message.
        void addToken(TokenLine& _target,
                      const std::string_view& _candidate,
                      const unsigned _linenum,
                      const unsigned _linepos,
                      ErrorList& _errorlist) {
            if(_candidate.empty())
                return;
            const auto& token = _target.add(_candidate);
            if(token.type == TokenType::ERROR)
                _errorlist.add(_linenum,
                               _linepos + 1 - _candidate.length(),
                               std::string("Invalid token: ") + std::string(token.token));
        }

        /// @brief Add token constructed by buffer to line. buffer is cleared afterwards.
        void pushBuffer(TokenLine& _target,
                        StringViewBuffer& _buffer,
                        const unsigned _linenum,
                        const unsigned _linepos,
                        ErrorList& _errorlist) {
            addToken(_target, _buffer.toSV(), _linenum, _linepos, _errorlist);
            _buffer.softclear();
        }
    }


#define ADD_TOKEN(candidate) addToken(tokenline, candidate, linenum, linepos, errorlist)
#define PUSH_BUFFER pushBuffer(tokenline, tokenbuffer, linenum, linepos, errorlist)

    TokenLine Tokenizer::tokenizeLine(const SourceLine& _sourceline) {
        TokenLine tokenline;
        const std::string_view linecode(_sourceline.linecode);
        const auto linenum = _sourceline.linenum;
        const auto len = linecode.length();
        if(!len)
            return tokenline;

        StringViewBuffer tokenbuffer(linecode);

        for(auto linepos = 0u; linepos < len; ++linepos) {
            switch(linecode[linepos]) {
                case '#': // comment
                    PUSH_BUFFER;
                    ADD_TOKEN(linecode.substr(linepos));
                    linepos = len;
                    break;

                case ' ':
                case '\t':
                case ',': // delimiter
                    PUSH_BUFFER;
                    break;

                case ':':
                case '(':
                case ')':
                case '[':
                case ']': // symbol
                    PUSH_BUFFER;
                    ADD_TOKEN(linecode.substr(linepos, 1));
                    break;

                case '$': // prospective register
                    PUSH_BUFFER;
                    tokenbuffer.append(linepos);
                    break;


                case '\'': // prospective char literal
                    PUSH_BUFFER;
                    {
                        auto escape = true;
                        do {
                            const auto preview = linecode[linepos];
                            tokenbuffer.append(linepos);
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
                            ++linepos;
                        } while(linepos < len);
                    }
                    break;

                default:
                    tokenbuffer.append(linepos);
                    break;
            }
        }

        pushBuffer(tokenline, tokenbuffer, linenum, len, errorlist);

        return tokenline;
    }
}
