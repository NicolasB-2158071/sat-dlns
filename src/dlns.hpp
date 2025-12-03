#pragma once

#include "clause.hpp"
#include "lbool.hpp"
#include "occurence_list.hpp"

#include <string>

class DLNS
{
public:
    DLNS();
    void init(int n_variables, int n_clauses);

    void solve(const std::vector<Lit> &assumptions);
    void search();
    void propagate();

    void add_clause(Clause &&clause);

    void assign(Lit lit, bool decision = false);
    int n_assigns();
    Lbool value(Lit lit) const;

    void undo(); // chronological backtrack (1-level)
    void raise_conflict();
    int get_random_free_variable() const;

    std::string output() const;

private:
    std::vector<Clause> m_clauses; // all clauses
    OccurenceList m_occ;           // watchers or adjacency list

    std::vector<Lbool> m_assigns;          // indexed on var
    std::vector<Lit> m_trail;              // of true lits
    std::vector<int> m_decision_variables; // int is index of var
    int m_trail_level;                     // index in trail
    bool m_conflict;                       // true == conflict

    enum class OUTPUT
    {
        SATISFIABLE,
        UNSATISFIABLE,
        UNKNOWN
    };

    OUTPUT m_output;
};