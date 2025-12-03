#include "lit.hpp"

Lit::Lit(int index, bool sign) : m_value{(index << 1) + int(sign)} {}

int Lit::index() const
{
    return m_value >> 1;
}

bool Lit::sign() const
{
    return m_value & 1;
}

bool Lit::operator==(const Lit &l) const
{
    return m_value == l.m_value;
}

Lit Lit::neg() const
{
    return Lit{this->index(), !this->sign()};
}