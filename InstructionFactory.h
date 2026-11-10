#pragma once
#include "Instruction.h"

class InstructionFactory
{
public:
    static bool validate(short raw, std::string &ErrMsg);
    static ExecutionResult execute(short raw, RegisterFile &registers, Memory &memory, Screen &screen);
};
