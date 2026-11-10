#include "UI.h"
#include <iostream>
#include <limits>
using namespace std;

UI::UI(VoleMachine &m) : vm(m) {}

bool UI::readInt(const char *msg, int &val, string &ErrMsg)
{
    ErrMsg.clear();
    cout << msg;
    if (cin >> val)
        return true;

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    ErrMsg = "Please enter a whole number.";
    return false;
}

void UI::showMenu() const
{
    cout << "\n========== VOLE MACHINE SIMULATOR ==========\n"
         << "1. Load a program from a file\n"
         << "2. Run the program\n"
         << "3. Run one instruction\n"
         << "4. Change a register\n"
         << "5. Change a memory cell\n"
         << "6. Show machine status\n"
         << "7. Show memory\n"
         << "8. Show screen output\n"
         << "9. Change the program counter\n"
         << "10. Reset the machine\n"
         << "0. Exit\n";
}

bool UI::loadProg(string &ErrMsg)
{
    ErrMsg.clear();
    string fn;
    cout << "Program file to load: ";
    if (!(cin >> fn))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        ErrMsg = "Please enter a program file name.";
        return false;
    }

    int st = 0;
    if (!readInt("Start address (even decimal number, 0-254): ", st, ErrMsg))
        return false;
    if (st < 0 || st > 254)
    {
        ErrMsg = "Start address must be from 0 to 254.";
        return false;
    }
    if (!vm.loadPrg(fn, st, ErrMsg))
        return false;

    cout << "Program loaded at address " << st << ".\n";
    vm.showMem();
    vm.showStat();
    return true;
}

bool UI::editReg(string &ErrMsg)
{
    ErrMsg.clear();
    int reg = 0;
    int val = 0;
    if (!readInt("Register number (0-15): ", reg, ErrMsg) ||
        !readInt("Value (0-255): ", val, ErrMsg))
        return false;
    if (reg < 0 || reg > 15)
    {
        ErrMsg = "Register number must be between 0 and 15.";
        return false;
    }
    if (val < 0 || val > 255)
    {
        ErrMsg = "Register value must be between 0 and 255.";
        return false;
    }
    if (!vm.setReg(reg, val, ErrMsg))
        return false;

    cout << "Register R" << reg << " updated.\n";
    return true;
}

bool UI::editMem(string &ErrMsg)
{
    ErrMsg.clear();
    int ad = 0;
    int val = 0;
    if (!readInt("Memory address (0-255): ", ad, ErrMsg) ||
        !readInt("Value (0-255): ", val, ErrMsg))
        return false;
    if (ad < 0 || ad > 255)
    {
        ErrMsg = "Memory address must be between 0 and 255.";
        return false;
    }
    if (val < 0 || val > 255)
    {
        ErrMsg = "Memory value must be between 0 and 255.";
        return false;
    }

    vm.setMem(ad, val);
    cout << "Memory cell " << ad << " updated.\n";
    return true;
}

bool UI::doChoice(int sel, string &ErrMsg)
{
    ErrMsg.clear();
    switch (sel)
    {
    case 1:
        return loadProg(ErrMsg);
    case 2:
        if (!vm.run(ErrMsg))
            return false;
        vm.showStat();
        vm.showScr();
        return true;
    case 3:
        if (!vm.step(ErrMsg))
            return false;
        vm.showStat();
        vm.showScr();
        return true;
    case 4:
        return editReg(ErrMsg);
    case 5:
        return editMem(ErrMsg);
    case 6:
        vm.showStat();
        return true;
    case 7:
        vm.showMem();
        return true;
    case 8:
        vm.showScr();
        return true;
    case 9:
    {
        int pc = 0;
        if (!readInt("New program counter (even address, 0-254): ", pc, ErrMsg))
            return false;
        if (pc < 0 || pc > 254)
        {
            ErrMsg = "Program counter must be between 0 and 254.";
            return false;
        }
        if (!vm.getCPU().setPC(pc, ErrMsg))
            return false;
        cout << "Program counter updated.\n";
        return true;
    }
    case 10:
        vm.reset();
        cout << "Machine reset.\n";
        return true;
    default:
        cout << "Invalid menu choice.\n";
        return true;
    }
}

void UI::run()
{
    bool run = true;
    while (run)
    {
        showMenu();
        string ErrMsg;
        int sel = 0;
        if (!readInt("Choose: ", sel, ErrMsg))
        {
            cout << "Error: " << ErrMsg << '\n';
            continue;
        }
        if (sel == 0)
        {
            cout << "Goodbye, and thanks for using our Vole Machine simulator!\n";
            run = false;
        }
        else if (!doChoice(sel, ErrMsg))
        {
            cout << "Error: " << ErrMsg << '\n';
        }
    }
}
