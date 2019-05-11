#ifndef C_FORMAT_CHECKER_LINE_HPP
#define C_FORMAT_CHECKER_LINE_HPP

#include "state.hpp"
#include "indent.hpp"
#include <string>


class Line : public std::vector<Token> {
    Indent indent;
public:
    explicit Line(Indent indent);
    void reset_indent(Indent new_indent);
    explicit operator std::string() const;
};


#endif //C_FORMAT_CHECKER_LINE_HPP