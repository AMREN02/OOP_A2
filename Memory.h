#pragma once
#include <array>
#include <cstdint>

class Memory
{
private:
    std::array<uint8_t, 256> mem{};

public:
    uint8_t get(uint8_t adr) const;
    void set(uint8_t adr, uint8_t val);
    void clear();
    void display() const;
};
