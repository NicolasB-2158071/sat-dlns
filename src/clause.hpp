#pragma once

#include "lit.hpp"
#include <vector>

class DLNS;

class Clause
{
public:
    Clause(std::vector<Lit> &&lits);
    std::vector<Lit> lits;
};