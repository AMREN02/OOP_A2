#include "ProgramLoader.h"
#include "InstructionFactory.h"
#include <cctype>
#include <fstream>
using namespace std;

bool ProgramLoader::loadFromFile(const string &fn, Memory &mem,
                                 uint8_t st, uint8_t &adr, string &ErrMsg)
{
    ErrMsg.clear();
    ifstream in(fn);
    if (!in)
    {
        ErrMsg = "Cannot open program file: " + fn;
        return false;
    }

    vector<string> lns;
    string ln;
    while (getline(in, ln))
        lns.push_back(ln);

    if (!loadFromLines(lns, mem, st, ErrMsg))
        return false;

    adr = st;
    return true;
}

bool ProgramLoader::parseHex(const string &hex, short &val, string &ErrMsg)
{
    ErrMsg.clear();
    if (hex.size() != 4)
    {
        ErrMsg = "Each instruction must contain exactly four hexadecimal digits.";
        return false;
    }

    val = 0;
    for (char ch : hex)
    {
        val <<= 4;
        if (ch >= '0' && ch <= '9')
            val |= ch - '0';
        else if (ch >= 'a' && ch <= 'f')
            val |= ch - 'a' + 10;
        else if (ch >= 'A' && ch <= 'F')
            val |= ch - 'A' + 10;
        else
        {
            ErrMsg = "Instruction contains a non-hexadecimal character.";
            return false;
        }
    }
    return true;
}

bool ProgramLoader::isValidIns(const string &hex)
{
    short ins = 0;
    string ErrMsg;
    if (!parseHex(hex, ins, ErrMsg))
        return false;
    return InstructionFactory::validate(ins, ErrMsg);
}

bool ProgramLoader::loadFromLines(const vector<string> &lns, Memory &mem,
                                  uint8_t st, string &ErrMsg)
{
    ErrMsg.clear();
    if (st % 2 != 0)
    {
        ErrMsg = "Program start address must be even.";
        return false;
    }
    if (st == 0xFF)
    {
        ErrMsg = "Program cannot start at the reserved screen address FF.";
        return false;
    }

    vector<uint8_t> buf;
    int i = 0;
    for (string ln : lns)
    {
        ++i;
        const size_t cmt = ln.find(';');
        if (cmt != string::npos)
            ln.erase(cmt);

        string txt;
        for (unsigned char ch : ln)
        {
            if (!isspace(ch))
                txt.push_back(ch);
        }
        if (txt.empty())
            continue;

        short ins = 0;
        if (!parseHex(txt, ins, ErrMsg) ||
            !InstructionFactory::validate(ins, ErrMsg))
        {
            ErrMsg = "Line " + to_string(i) + ": " + ErrMsg;
            return false;
        }

        buf.push_back((ins >> 8) & 0xFF);
        buf.push_back(ins & 0xFF);
    }

    const size_t cap = 0xFF - st;
    if (buf.size() > cap)
    {
        ErrMsg = "Program is too large.";
        return false;
    }

    for (size_t j = 0; j < buf.size(); ++j)
        mem.set(st + j, buf[j]);
    return true;
}
