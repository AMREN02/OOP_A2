#include "Screen.h"
#include <iomanip>
#include <iostream>
#include <sstream>
using namespace std;

void Screen::write(uint8_t val)
{
    ostringstream out;
    out << uppercase << hex << setw(2) << setfill('0') << +val;
    if (!txt.empty())
        txt += ' ';
    txt += out.str();
}
void Screen::clear() { txt.clear(); }
const string &Screen::getText() const { return txt; }

void Screen::display() const
{
    cout << "\nScreen output (hexadecimal): "
         << (txt.empty() ? "nothing yet" : txt) << '\n';
}
