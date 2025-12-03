#include "clause.hpp"
#include "dlns.hpp"

Clause::Clause(std::vector<Lit> &&lits) : lits{lits} {}