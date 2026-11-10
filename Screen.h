#pragma once
#include <cstdint>
#include <string>

class Screen
{
private:
    std::string txt;

public:
    void write(uint8_t val);
    void clear();
    const std::string &getText() const;
    void display() const;
};
