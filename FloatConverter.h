#pragma once
#include <cstdint>

struct FloatParts
{
    bool sgn = false;
    int exp = 0;
    int man = 0;
};

class FloatConverter
{
public:
    static FloatParts decode(uint8_t val);
    static uint8_t encode(const FloatParts &p);
    static double toNum(uint8_t val);
    static uint8_t add(uint8_t a, uint8_t b);
};
