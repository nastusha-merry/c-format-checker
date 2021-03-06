#include <sstream>
#include "parser.hpp"


Parser::Parser(Scanner &scanner, const Params &params) :
    scanner(scanner), params(params)
{
    lines.emplace_back(Indent());
}

Parser::State Parser::saveState()
{
    state = {cur_indent, token, last_token, lines, rule_cases, last_case, scanner.saveState(), errors_list};
    return state;
}

void Parser::restoreState()
{
    cur_indent = state.cur_indent;
    token      = state.token;
    last_token = state.last_token;
    lines      = state.lines;
    rule_cases = state.rule_cases;
    last_case  = state.last_case;
    scanner.restoreState(state.scanner_state);
    errors_list = state.errors_list;
}

Parser::State Parser::restoreState(const Parser::State &backup)
{
    saveState();
    cur_indent = backup.cur_indent;
    token      = backup.token;
    last_token = backup.last_token;
    lines      = backup.lines;
    rule_cases = backup.rule_cases;
    last_case  = backup.last_case;
    scanner.restoreState(backup.scanner_state);
    errors_list = backup.errors_list;
    return state;
}



void Parser::readNewlines()
{
    while (token == lexem::NEWLINE) {
        cur_indent.update(token);
        nextToken();
    }
}

//const Token& Parser::nextToken()
//{
//
//    token = scanner.nextToken();
//    if (token == lexem::NEWLINE) {
//        cur_indent.update(token);
//    }
//    std::cout << token << std::endl;
//    return token;
//}


const Token& Parser::nextTokenPragma()
{
    last_token = token;
    token = scanner.nextToken();
    if (token == lexem::Type::NEWLINE) {
        cur_indent.update(token);
        if (lines.back().empty()) {
            lines.pop_back();
        }
        lines.emplace_back(cur_indent);
    }
//    std::cout << token << std::endl;
    return token;
}

const Token& Parser::nextToken()
{
    last_token = token;
    token = scanner.nextToken();
    if (token == lexem::Type::NEWLINE) {
        while (token == lexem::Type::NEWLINE) {
            cur_indent.update(token);
            token = scanner.nextToken();
        }
        lines.addLine(cur_indent, rule_cases);
    }
    switch (token.type()) {
        case lexem::Type::BACKSLASH:
            return nextToken();
        case lexem::Type::HASH:
            parse_pragma();
            return token;
        default:
            lines.pushToken(token);
            return token;
    }
}

std::string_view Parser::get_image(Coords start, Coords follow) const
{
    return scanner.substring(start, follow);
}

std::string Parser::get_lines() const
{
    std::stringstream ss;
    ss << "line:  indent [state] {tokens}\n";
    for (const Line &line: lines) {
        ss << std::string(line) << "\n";
    }
    return ss.str();
}

Rules::Cases Parser::popCase(bool correct_current_token)
{
    last_case = rule_cases.back();
    rule_cases.pop_back();
    if (correct_current_token) {
        lines.correctState(rule_cases);
//        if (lines.back().size() == 1) {  // one token in line => correct line state (exclude follow token from rule)
//            lines.back().popState();
//        }
    }
    return last_case;
}