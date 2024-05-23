#pragma once
#include <cstdio>
#include <iostream>
#include <string>

#include "testscript.cpp"

class ShellTestApp {
public:
    ShellTestApp(SSD* pSsd);

    void write(unsigned int nLba, unsigned int nData);

    void read(unsigned int nLba);

    void exit();

    void help();

    void fullWrite(unsigned int nData);

    void fullRead();

    void doScript(std::string sTestScriptName);

private:
    const unsigned int MAX_LBA_NUM = 100;
    const int CYCLE = 3;
    
    TestScript* testScript;
    SSD* pSsd;

    FILE* fpResult;
};
