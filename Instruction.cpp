#include "Instruction.h"
#include "FloatConverter.h"
#include <iomanip>
#include <sstream>
using namespace std;

Instruction::Instruction(short w)
    : op((w >> 12) & 0x0F),
      raw(w)
{
}

uint8_t Instruction::getOp() const
{
    return op;
}

short Instruction::getString() const
{
    return raw;
}

string Instruction::toHex() const
{
    ostringstream out;
    out << uppercase << hex << setw(4) << setfill('0') << raw;
    return out.str();
}

namespace
{
    constexpr uint8_t SCR = 0xFF;

    ExecutionResult failed(const string &ErrMsg)
    {
        return {ExecutionKind::CONTINUE, 0, false, ErrMsg};
    }

    bool readReg(RegisterFile &rf, uint8_t reg, uint8_t &val, string &ErrMsg)
    {
        return rf.get(reg, val, ErrMsg);
    }

    bool writeReg(RegisterFile &rf, uint8_t reg, uint8_t val, string &ErrMsg)
    {
        return rf.set(reg, val, ErrMsg);
    }
}

LoadFromMemory::LoadFromMemory(short raw) : Instruction(raw) {}
ExecutionResult LoadFromMemory::execute(RegisterFile &rf, Memory &mem, Screen &) const
{
    const uint8_t reg = (raw >> 8) & 0x0F;
    const uint8_t ad = raw & 0xFF;
    string ErrMsg;
    if (!writeReg(rf, reg, mem.get(ad), ErrMsg))
        return failed(ErrMsg);
    return {};
}

LoadImmediate::LoadImmediate(short raw) : Instruction(raw) {}
ExecutionResult LoadImmediate::execute(RegisterFile &rf, Memory &, Screen &) const
{
    const uint8_t reg = (raw >> 8) & 0x0F;
    const uint8_t val = raw & 0xFF;
    string ErrMsg;
    if (!writeReg(rf, reg, val, ErrMsg))
        return failed(ErrMsg);
    return {};
}

Store::Store(short raw) : Instruction(raw) {}
ExecutionResult Store::execute(RegisterFile &rf, Memory &mem, Screen &scr) const
{
    const uint8_t reg = (raw >> 8) & 0x0F;
    const uint8_t ad = raw & 0xFF;
    uint8_t val = 0;
    string ErrMsg;
    if (!readReg(rf, reg, val, ErrMsg))
        return failed(ErrMsg);
    if (ad == SCR)
        scr.write(val);
    else
        mem.set(ad, val);
    return {};
}

Move::Move(short raw) : Instruction(raw) {}
ExecutionResult Move::execute(RegisterFile &rf, Memory &, Screen &) const
{
    const uint8_t src = (raw >> 4) & 0x0F;
    const uint8_t dst = raw & 0x0F;
    uint8_t val = 0;
    string ErrMsg;
    if (!readReg(rf, src, val, ErrMsg) ||
        !writeReg(rf, dst, val, ErrMsg))
        return failed(ErrMsg);
    return {};
}

AddInteger::AddInteger(short raw) : Instruction(raw) {}
ExecutionResult AddInteger::execute(RegisterFile &rf, Memory &, Screen &) const
{
    const uint8_t dst = (raw >> 8) & 0x0F;
    const uint8_t s1 = (raw >> 4) & 0x0F;
    const uint8_t s2 = raw & 0x0F;
    uint8_t v1 = 0;
    uint8_t v2 = 0;
    string ErrMsg;
    if (!readReg(rf, s1, v1, ErrMsg) ||
        !readReg(rf, s2, v2, ErrMsg) ||
        !writeReg(rf, dst, v1 + v2, ErrMsg))
        return failed(ErrMsg);
    return {};
}

AddFloat::AddFloat(short raw) : Instruction(raw) {}
ExecutionResult AddFloat::execute(RegisterFile &rf, Memory &, Screen &) const
{
    const uint8_t dst = (raw >> 8) & 0x0F;
    const uint8_t s1 = (raw >> 4) & 0x0F;
    const uint8_t s2 = raw & 0x0F;
    uint8_t v1 = 0;
    uint8_t v2 = 0;
    string ErrMsg;
    if (!readReg(rf, s1, v1, ErrMsg) ||
        !readReg(rf, s2, v2, ErrMsg) ||
        !writeReg(rf, dst, FloatConverter::add(v1, v2), ErrMsg))
        return failed(ErrMsg);
    return {};
}

Or::Or(short raw) : Instruction(raw) {}
ExecutionResult Or::execute(RegisterFile &rf, Memory &, Screen &) const
{
    const uint8_t dst = (raw >> 8) & 0x0F;
    const uint8_t s1 = (raw >> 4) & 0x0F;
    const uint8_t s2 = raw & 0x0F;
    uint8_t v1 = 0;
    uint8_t v2 = 0;
    string ErrMsg;
    if (!readReg(rf, s1, v1, ErrMsg) ||
        !readReg(rf, s2, v2, ErrMsg) ||
        !writeReg(rf, dst, v1 | v2, ErrMsg))
        return failed(ErrMsg);
    return {};
}

And::And(short raw) : Instruction(raw) {}
ExecutionResult And::execute(RegisterFile &rf, Memory &, Screen &) const
{
    const uint8_t dst = (raw >> 8) & 0x0F;
    const uint8_t s1 = (raw >> 4) & 0x0F;
    const uint8_t s2 = raw & 0x0F;
    uint8_t v1 = 0;
    uint8_t v2 = 0;
    string ErrMsg;
    if (!readReg(rf, s1, v1, ErrMsg) ||
        !readReg(rf, s2, v2, ErrMsg) ||
        !writeReg(rf, dst, v1 & v2, ErrMsg))
        return failed(ErrMsg);
    return {};
}

Xor::Xor(short raw) : Instruction(raw) {}
ExecutionResult Xor::execute(RegisterFile &rf, Memory &, Screen &) const
{
    const uint8_t dst = (raw >> 8) & 0x0F;
    const uint8_t s1 = (raw >> 4) & 0x0F;
    const uint8_t s2 = raw & 0x0F;
    uint8_t v1 = 0;
    uint8_t v2 = 0;
    string ErrMsg;
    if (!readReg(rf, s1, v1, ErrMsg) ||
        !readReg(rf, s2, v2, ErrMsg) ||
        !writeReg(rf, dst, v1 ^ v2, ErrMsg))
        return failed(ErrMsg);
    return {};
}

RotateRight::RotateRight(short raw) : Instruction(raw) {}
ExecutionResult RotateRight::execute(RegisterFile &rf, Memory &, Screen &) const
{
    const uint8_t reg = (raw >> 8) & 0x0F;
    const unsigned cnt = (raw & 0x0F) % 8;
    uint8_t val = 0;
    string ErrMsg;
    if (!readReg(rf, reg, val, ErrMsg))
        return failed(ErrMsg);
    const uint8_t res = cnt == 0 ? val : (val >> cnt) | (val << (8 - cnt));
    if (!writeReg(rf, reg, res, ErrMsg))
        return failed(ErrMsg);
    return {};
}

JumpEqual::JumpEqual(short raw) : Instruction(raw) {}
ExecutionResult JumpEqual::execute(RegisterFile &rf, Memory &, Screen &) const
{
    const uint8_t reg = (raw >> 8) & 0x0F;
    const uint8_t tgt = raw & 0xFF;
    uint8_t val = 0;
    uint8_t zv = 0;
    string ErrMsg;
    if (!readReg(rf, reg, val, ErrMsg) ||
        !readReg(rf, 0, zv, ErrMsg))
        return failed(ErrMsg);
    if (val == zv)
    {
        if (tgt % 2 != 0)
            return failed("Jump target must be an even instruction address.");
        return {ExecutionKind::JUMP, tgt, true, {}};
    }
    return {};
}

Halt::Halt(short raw) : Instruction(raw) {}
ExecutionResult Halt::execute(RegisterFile &, Memory &, Screen &) const
{
    return {ExecutionKind::HALT, 0, true, {}};
}
