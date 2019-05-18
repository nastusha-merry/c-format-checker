#ifndef C_FORMAT_CHECKER_PARSETABLE_HPP
#define C_FORMAT_CHECKER_ANALYZER_HPP

#include "line.hpp"
#include "state_vector.hpp"
#include <vector>
#include <list>
#include <string>
#include <map>

class Analyzer: public std::vector<Line> {
    Rules rules;
    std::map<StateVector, std::vector<Indent>> stats;
    std::list<std::string> error_list;
    std::string wrap_error(const StateVector &state, const Indent &err_ind, const Indent &standard
                            , const std::string &level="error", const std::string &assumption="") const;
    std::string wrap_error(const StateVector &state, const Indent &err_ind
                            , const std::string &level="error", const std::string &assumption="") const;
public:
    std::string str_stats() const;
    explicit Analyzer(const std::vector<Line> &other, Rules rules);
    explicit operator std::string() const;
    void first_pass();
    void collect_stats();
    void analyze();
    std::string error_messages() const;

};


#endif //C_FORMAT_CHECKER_PARSETABLE_HPP
