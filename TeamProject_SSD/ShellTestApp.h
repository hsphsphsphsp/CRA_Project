#pragma once
#include <cstdio>
#include <string>

#include "ITestScript.h"

class ShellTestApp {
public:
    void write(unsigned int nLba, unsigned int nData);

    void read(unsigned int nLba);

    void exit();

    void help();

    void fullWrite(unsigned int nData);

    void fullRead();

    void doScript(std::string sTestScriptName);

private:
    const unsigned int MAX_LBA_NUM = 100;
    
    ITestScript* testScript;

    FILE* fpResult;
};
