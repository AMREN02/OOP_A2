#include "Memory.h"
#include <iomanip>
#include <iostream>
using namespace std;

uint8_t Memory::get(uint8_t adr) const { return mem[adr]; }
void Memory::set(uint8_t adr, uint8_t val) { mem[adr] = val; }
void Memory::clear() { mem.fill(0); }

void Memory::display() const
{
    cout << "\nMemory (hexadecimal)\n     ";
    for (int col = 0; col < 16; ++col)
        cout << uppercase << hex << setw(2) << setfill('0') << col << "  ";
    cout << '\n';
    for (int row = 0; row < 16; ++row)
    {
        cout << setw(2) << row * 16 << " : ";
        for (int col = 0; col < 16; ++col)
            cout << setw(2) << +mem[row * 16 + col] << "  ";
        cout << '\n';
    }
    cout << dec;
}
