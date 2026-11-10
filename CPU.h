#pragma once
#include <cstdint>
#include <string>
#include "RegisterFile.h"
#include "Memory.h"
#include "Screen.h"

class CPU
{
private:
    RegisterFile regs;
    uint8_t pc = 0;
    short ir = 0;
    bool halted = false;

public:
    bool fetch(Memory &mem, std::string &ErrMsg);
    bool step(Memory &mem, Screen &scr, std::string &ErrMsg);
    void reset();
    bool isHalted() const;
    uint8_t getPC() const;
    bool setPC(uint8_t addr, std::string &ErrMsg);
    short getIR() const;
    RegisterFile &getRegs();
    const RegisterFile &getRegs() const;
};
