#pragma once

#include <string>
#include "lit.hpp"

class DLNS;

class Dimacs
{
public:
    void parse(const std::string &path, DLNS &solver);

private:
    std::string next_word(std::ifstream &file) const;
    void skip_line(std::ifstream &file) const;
    void read_problem(std::ifstream &file, DLNS &solver);
    void read_clause(std::ifstream &file, const std::string &first_word, DLNS &solver) const;

    bool m_problem_read;
};

// https://people.sc.fsu.edu/~jburkardt/data/cnf/cnf.html