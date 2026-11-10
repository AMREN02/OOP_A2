#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "Memory.h"
using namespace std;

class ProgramLoader
{
public:
    static bool loadFromFile(const string &filename, Memory &mem, uint8_t startAddr,
                             uint8_t &loadedAt, string &ErrMsg);
    static bool loadFromLines(const vector<string> &lines, Memory &mem, uint8_t startAddr, string &ErrMsg);
    static bool isValidIns(const string &hex);
    static bool parseHex(const string &hex, short &value, string &ErrMsg);
};
