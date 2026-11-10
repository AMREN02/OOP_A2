#pragma once
#include "VoleMachine.h"
#include <string>

class UI
{
private:
    VoleMachine &vm;
    static bool readInt(const char *msg, int &val, std::string &ErrMsg);
    bool loadProg(std::string &ErrMsg);
    bool editReg(std::string &ErrMsg);
    bool editMem(std::string &ErrMsg);
    bool doChoice(int sel, std::string &ErrMsg);

public:
    UI(VoleMachine &machine);
    void showMenu() const;
    void run();
};
