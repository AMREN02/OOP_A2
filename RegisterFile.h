#pragma once
#include <array>
#include <cstdint>
#include <string>

class RegisterFile
{
private:
    std::array<uint8_t, 16> registers{};

public:
    bool get(uint8_t reg, uint8_t &val, std::string &ErrMsg) const;
    bool set(uint8_t reg, uint8_t val, std::string &ErrMsg);
    void clear();
    void display() const;
};
