#pragma once

#include <vector>

class Lit
{
public:
    Lit(int index, bool sign); // false sign == no complement

    int index() const;
    bool sign() const;
    Lit neg() const;

    bool operator==(const Lit &l) const;

private:
    int m_value;
};

// Simple datastructure allows for passing by value