#ifndef SCANNER_H
#define SCANNER_H

#include "frontend/lexer/token.hpp"
#include "frontend/lexer/coords.hpp"
#include <list>
#include <string>
#include <unordered_map>

class Scanner {
    std::string program;
    std::unordered_map<std::string_view, lexem::Type> keywords2type;
    const char *start_ptr = nullptr;
    const char *cur_ptr = nullptr;
    Coords start_pos;
    Coords cur_pos;
    char  cur_char;
    int   token_len;

    std::string_view image() const;
    enum CommentStyle {
        ONELINE, MULTYLINE
    };
    void  skipComment(CommentStyle type);
    void  skipWhitespaces();
    bool  reachedEOF();
    Token scanWhitespaces();
    Token scanIdent();
    Token scanNum();
    Token scanString();
    Token scanChar();
    void  nextChar();
    char  peekNext();
    void  clearCounters();
    void  rollbackCounters();
    Token tokenOfType(lexem::Type type);
    void  add_error(std::string message);
    std::list<std::string> errors_list;
public:
    Scanner(const char *path);
    Token nextToken();
    void  print_errors();
};

#endif // SCANNER_H
