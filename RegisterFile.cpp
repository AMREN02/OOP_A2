#include "RegisterFile.h"
#include <iomanip>
#include <iostream>
using namespace std;

bool RegisterFile::get(uint8_t reg, uint8_t &val, string &ErrMsg) const
{
    ErrMsg.clear();
    if (reg > 0x0F)
    {
        ErrMsg = "Register number must be between 0 and 15.";
        return false;
    }
    val = registers[reg];
    return true;
}
bool RegisterFile::set(uint8_t reg, uint8_t val, string &ErrMsg)
{
    ErrMsg.clear();
    if (reg > 0x0F)
    {
        ErrMsg = "Register number must be between 0 and 15.";
        return false;
    }
    registers[reg] = val;
    return true;
}
void RegisterFile::clear() { registers.fill(0); }
void RegisterFile::display() const
{
    cout << "\nRegisters\n";
    for (int i = 0; i < 16; ++i)
    {
        cout << 'R' << uppercase << hex << i << " = " << setw(2) << setfill('0') << +registers[i];
        cout << ((i % 4 == 3) ? '\n' : '\t');
    }
    cout << dec;
}
