#pragma once
#include <cstdint>

// Stripped down version of Minisat lbool

class Lbool
{
public:
    Lbool() : value(0) {}
    explicit Lbool(uint8_t v) : value(v) {}
    explicit Lbool(bool x) : value(!x) {}

    bool operator==(Lbool b) const { return ((b.value & 2) & (value & 2)) | (!(b.value & 2) & (value == b.value)); }
    bool operator!=(Lbool b) const { return !(*this == b); }
    Lbool operator^(bool b) const { return Lbool((uint8_t)(value ^ (uint8_t)b)); }

private:
    uint8_t value;
};

const Lbool l_true((uint8_t)0);
const Lbool l_false((uint8_t)1);
const Lbool l_undef((uint8_t)2);