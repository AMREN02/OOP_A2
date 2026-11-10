#include "FloatConverter.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
using namespace std;

FloatParts FloatConverter::decode(uint8_t val)
{
    FloatParts p;
    p.sgn = (val & 0x80) != 0;
    p.exp = ((val >> 4) & 0x07) - 4;
    p.man = val & 0x0F;
    return p;
}

uint8_t FloatConverter::encode(const FloatParts &p)
{
    int exp = p.exp;
    int man = p.man;
    if (exp < -4)
        exp = -4;
    else if (exp > 3)
        exp = 3;
    if (man < 0)
        man = 0;
    else if (man > 15)
        man = 15;
    if (man == 0)
        return 0;
    const int sgn = p.sgn ? 0x80 : 0;
    const int eb = exp + 4;
    return sgn | (eb << 4) | man;
}

double FloatConverter::toNum(uint8_t val)
{
    const FloatParts p = decode(val);
    const double frc = p.man / 16.0;
    const double mag = ldexp(frc, p.exp);
    return p.sgn ? -mag : mag;
}

uint8_t FloatConverter::add(uint8_t a, uint8_t b)
{
    FloatParts l = decode(a);
    FloatParts r = decode(b);
    if (l.man == 0)
        return r.man == 0 ? 0 : b;
    if (r.man == 0)
        return a;

    const int exp = max(l.exp, r.exp);
    int lm = l.man >> (exp - l.exp);
    int rm = r.man >> (exp - r.exp);
    if (l.sgn)
        lm = -lm;
    if (r.sgn)
        rm = -rm;

    int sum = lm + rm;
    if (sum == 0)
        return 0;
    const bool sg = sum < 0;
    int man = abs(sum);
    int oe = exp;

    while (man >= 16 && oe < 3)
    {
        man >>= 1;
        ++oe;
    }
    while (man > 0 && man < 8 && oe > -4)
    {
        man <<= 1;
        --oe;
    }

    if (oe > 3)
    {
        oe = 3;
        man = 15;
    }
    if (oe < -4 || man == 0)
        return 0;
    if (man > 15)
        man = 15;
    return encode(FloatParts{sg, oe, man});
}
