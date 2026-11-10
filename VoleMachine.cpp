#include "VoleMachine.h"
#include "ProgramLoader.h"
#include <iomanip>
#include <iostream>
using namespace std;

bool VoleMachine::loadPrg(const string &fn, uint8_t st, string &ErrMsg)
{
    ErrMsg.clear();
    uint8_t adr = 0;
    if (!ProgramLoader::loadFromFile(fn, mem, st, adr, ErrMsg))
        return false;

    cpu.reset();
    if (!cpu.setPC(adr, ErrMsg))
        return false;
    scr.clear();
    return true;
}

bool VoleMachine::step(string &ErrMsg)
{
    ErrMsg.clear();
    if (cpu.isHalted())
        return true;
    return cpu.step(mem, scr, ErrMsg);
}

bool VoleMachine::run(string &ErrMsg)
{
    ErrMsg.clear();
    constexpr int lim = 10000;
    int n = 0;
    while (!cpu.isHalted() && n < lim)
    {
        if (!step(ErrMsg))
            return false;
        ++n;
    }
    if (n == lim && !cpu.isHalted())
        cout << "The program reached the 10,000-step safety limit. It may be stuck in a loop.\n";
    return true;
}

void VoleMachine::reset()
{
    mem.clear();
    cpu.reset();
    scr.clear();
}

void VoleMachine::showStat() const
{
    cout << "\nMachine status\n"
         << "Program counter: " << uppercase << hex << setw(2) << setfill('0')
         << +cpu.getPC() << "   Instruction: " << setw(4) << cpu.getIR()
         << "   State: " << (cpu.isHalted() ? "stopped" : "running") << '\n'
         << dec;
    cpu.getRegs().display();
}
void VoleMachine::showMem() const { mem.display(); }
void VoleMachine::showRegs() const { cpu.getRegs().display(); }
void VoleMachine::showScr() const { scr.display(); }
Memory &VoleMachine::getMem() { return mem; }
CPU &VoleMachine::getCPU() { return cpu; }
Screen &VoleMachine::getScr() { return scr; }
bool VoleMachine::setReg(uint8_t reg, uint8_t val, string &ErrMsg)
{
    ErrMsg.clear();
    return cpu.getRegs().set(reg, val, ErrMsg);
}
void VoleMachine::setMem(uint8_t ad, uint8_t val) { mem.set(ad, val); }
