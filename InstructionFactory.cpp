#include "InstructionFactory.h"
using namespace std;

namespace
{
    template <typename Op>
    ExecutionResult runOp(short raw, RegisterFile &rf, Memory &mem, Screen &scr)
    {
        const Op obj(raw);
        return obj.execute(rf, mem, scr);
    }
}

bool InstructionFactory::validate(short raw, string &ErrMsg)
{
    ErrMsg.clear();
    const uint8_t op = (raw >> 12) & 0x0F;
    switch (op)
    {
    case 0x1:
    case 0x2:
    case 0x3:
    case 0x5:
    case 0x6:
    case 0x7:
    case 0x8:
    case 0x9:
    case 0xB:
        return true;
    case 0x4:
        if (((raw >> 8) & 0x0F) != 0)
        {
            ErrMsg = "Move format must be 40RS.";
            return false;
        }
        return true;
    case 0xA:
        if (((raw >> 4) & 0x0F) != 0)
        {
            ErrMsg = "Rotate format must be AR0X.";
            return false;
        }
        return true;
    case 0xC:
        if ((raw & 0x0FFF) != 0)
        {
            ErrMsg = "Halt instruction must be C000.";
            return false;
        }
        return true;
    default:
        ErrMsg = "wrong code. Enter an opcode from 1 to C.";
        return false;
    }
}

ExecutionResult InstructionFactory::execute(short raw, RegisterFile &rf, Memory &mem, Screen &scr)
{
    string ErrMsg;
    if (!validate(raw, ErrMsg))
        return {ExecutionKind::CONTINUE, 0, false, ErrMsg};
    const uint8_t op = (raw >> 12) & 0x0F;
    switch (op)
    {
    case 0x1:
        return runOp<LoadFromMemory>(raw, rf, mem, scr);
    case 0x2:
        return runOp<LoadImmediate>(raw, rf, mem, scr);
    case 0x3:
        return runOp<Store>(raw, rf, mem, scr);
    case 0x4:
        return runOp<Move>(raw, rf, mem, scr);
    case 0x5:
        return runOp<AddInteger>(raw, rf, mem, scr);
    case 0x6:
        return runOp<AddFloat>(raw, rf, mem, scr);
    case 0x7:
        return runOp<Or>(raw, rf, mem, scr);
    case 0x8:
        return runOp<And>(raw, rf, mem, scr);
    case 0x9:
        return runOp<Xor>(raw, rf, mem, scr);
    case 0xA:
        return runOp<RotateRight>(raw, rf, mem, scr);
    case 0xB:
        return runOp<JumpEqual>(raw, rf, mem, scr);
    case 0xC:
        return runOp<Halt>(raw, rf, mem, scr);
    default:
        return {ExecutionKind::CONTINUE, 0, false, "wrong code. Enter an opcode from 1 to C."};
    }
}
