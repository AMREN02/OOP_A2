#include "CPU.h"
#include "InstructionFactory.h"
using namespace std;

bool CPU::fetch(Memory &mem, string &ErrMsg)
{
    ErrMsg.clear();
    if (pc > 0xFE || pc % 2 != 0)
    {
        ErrMsg = "Program counter must be an even address between 00 and FE.";
        return false;
    }
    const uint8_t high = mem.get(pc);
    const uint8_t low = mem.get(pc + 1);
    ir = (high << 8) | low;
    return true;
}

bool CPU::step(Memory &mem, Screen &screen, string &ErrMsg)
{
    ErrMsg.clear();
    if (halted)
        return true;
    if (!fetch(mem, ErrMsg))
        return false;
    const uint8_t oldPC = pc;
    const ExecutionResult result = InstructionFactory::execute(ir, regs, mem, screen);
    if (!result.ok)
    {
        ErrMsg = result.ErrMsg;
        return false;
    }

    switch (result.kind)
    {
    case ExecutionKind::CONTINUE:
        if (oldPC > 0xFC)
        {
            ErrMsg = "Program counter cannot move beyond available instruction memory.";
            return false;
        }
        pc = oldPC + 2;
        break;
    case ExecutionKind::JUMP:
        if (!setPC(result.jmp, ErrMsg))
            return false;
        break;
    case ExecutionKind::HALT:
        halted = true;
        break;
    }
    return true;
}

void CPU::reset()
{
    regs.clear();
    pc = 0;
    ir = 0;
    halted = false;
}

bool CPU::isHalted() const
{
    return halted;
}

uint8_t CPU::getPC() const
{
    return pc;
}

bool CPU::setPC(uint8_t address, string &ErrMsg)
{
    ErrMsg.clear();
    if (address % 2 != 0 || address == 0xFF)
    {
        ErrMsg = "Program counter must be an even address and cannot be FF.";
        return false;
    }
    pc = address;
    halted = false;
    return true;
}

short CPU::getIR() const
{
    return ir;
}

RegisterFile &CPU::getRegs()
{
    return regs;
}

const RegisterFile &CPU::getRegs() const
{
    return regs;
}
