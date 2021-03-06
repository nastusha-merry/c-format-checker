#ifndef SCANNER_H
#define SCANNER_H

#include "frontend/lexer/token.hpp"
#include "frontend/lexer/coords.hpp"
#include "params.hpp"
#include <list>
#include <string>
#include <unordered_map>

class Scanner {
public:
    struct State {
        const char *start_ptr;
        const char *cur_ptr;
        Coords start_pos;
        Coords cur_pos;
        char  cur_char;
        int   token_len;
        std::list<std::string> errors_list;
    };

    const Params &params;
private:
    std::string program;
    std::unordered_map<std::string_view, lexem::Type> keywords2type;
    const char *start_ptr = nullptr;
    const char *cur_ptr = nullptr;
    Coords start_pos;
    Coords cur_pos;
    char  cur_char;
    int   token_len;

    std::list<std::string> errors_list;
    State state;

    std::string_view image() const;
    enum CommentStyle {
        ONE_LINE, MULTI_LINE
    };
    void  skipComment(CommentStyle type);
    void  skipWhitespaces();
//    Token scanWhitespaces();
    Token scanNewline();
    Token scanIdent();
    Token scanNum();
    Token scanString();
    Token scanChar();
    Token scanAngleOp();
    void  nextChar();
    char  peekNext();
    void  clearCounters();
    void  rollbackCounters();
    bool  reachedEOF();
    bool  isKeyword(std::string_view word) const;
    bool  isType(std::string_view word) const;
    Token tokenOfType(lexem::Type type);
    void  add_error(const std::string &message);
public:
    explicit Scanner(const Params &params);
    Token nextToken();
    Token peekToken(bool dotall = false);
    std::string error_messages();
    State saveState();
    void  restoreState();
    State restoreState(const State &backup);
    std::string_view substring(Coords start, Coords follow) const;
};

#endif // SCANNER_H
