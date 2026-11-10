#pragma once
#include <cstdint>
#include <string>
#include "RegisterFile.h"
#include "Memory.h"
#include "Screen.h"

enum class ExecutionKind
{
    CONTINUE,
    JUMP,
    HALT
};
struct ExecutionResult
{
    ExecutionKind kind = ExecutionKind::CONTINUE;
    uint8_t jmp = 0;
    bool ok = true;
    std::string ErrMsg;
};

class Instruction
{
protected:
    uint8_t op;
    short raw;

public:
    Instruction(short raw);
    virtual ~Instruction() = default;
    virtual ExecutionResult execute(RegisterFile &rf, Memory &mem, Screen &scr) const = 0;
    uint8_t getOp() const;
    short getString() const;
    virtual std::string toHex() const;
};

class LoadFromMemory : public Instruction
{
public:
    LoadFromMemory(short raw);
    ExecutionResult execute(RegisterFile &rf, Memory &mem, Screen &scr) const override;
};

class LoadImmediate : public Instruction
{
public:
    LoadImmediate(short raw);
    ExecutionResult execute(RegisterFile &rf, Memory &mem, Screen &scr) const override;
};

class Store : public Instruction
{
public:
    Store(short raw);
    ExecutionResult execute(RegisterFile &rf, Memory &mem, Screen &scr) const override;
};

class Move : public Instruction
{
public:
    Move(short raw);
    ExecutionResult execute(RegisterFile &rf, Memory &mem, Screen &scr) const override;
};

class AddInteger : public Instruction
{
public:
    AddInteger(short raw);
    ExecutionResult execute(RegisterFile &rf, Memory &mem, Screen &scr) const override;
};

class AddFloat : public Instruction
{
public:
    AddFloat(short raw);
    ExecutionResult execute(RegisterFile &rf, Memory &mem, Screen &scr) const override;
};

class Or : public Instruction
{
public:
    Or(short raw);
    ExecutionResult execute(RegisterFile &rf, Memory &mem, Screen &scr) const override;
};

class And : public Instruction
{
public:
    And(short raw);
    ExecutionResult execute(RegisterFile &rf, Memory &mem, Screen &scr) const override;
};

class Xor : public Instruction
{
public:
    Xor(short raw);
    ExecutionResult execute(RegisterFile &rf, Memory &mem, Screen &scr) const override;
};

class RotateRight : public Instruction
{
public:
    RotateRight(short raw);
    ExecutionResult execute(RegisterFile &rf, Memory &mem, Screen &scr) const override;
};

class JumpEqual : public Instruction
{
public:
    JumpEqual(short raw);
    ExecutionResult execute(RegisterFile &rf, Memory &mem, Screen &scr) const override;
};

class Halt : public Instruction
{
public:
    Halt(short raw);
    ExecutionResult execute(RegisterFile &rf, Memory &mem, Screen &scr) const override;
};
