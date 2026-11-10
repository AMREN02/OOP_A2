#pragma once
#include <cstdint>
#include <string>
#include "Memory.h"
#include "CPU.h"
#include "Screen.h"

class VoleMachine
{
private:
    Memory mem;
    CPU cpu;
    Screen scr;

public:
    bool loadPrg(const std::string &filename, uint8_t startAddress, std::string &ErrMsg);
    bool step(std::string &ErrMsg);
    bool run(std::string &ErrMsg);
    void reset();
    void showStat() const;
    void showMem() const;
    void showRegs() const;
    void showScr() const;
    Memory &getMem();
    CPU &getCPU();
    Screen &getScr();
    bool setReg(uint8_t reg, uint8_t val, std::string &ErrMsg);
    void setMem(uint8_t address, uint8_t val);
};
