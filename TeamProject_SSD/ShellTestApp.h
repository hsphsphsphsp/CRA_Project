#pragma once
#include <cstdio>
#include <string>

#include "ITestScript.h"

class ShellTestApp {
public:
    void write(char cLba, int iData);

    void read(char cLba);

    void exit();

    void help();

    void fullWrite(int iData);

    void fullRead();

    void doScript(std::string testScriptName);

private:
    const char MAX_LBA_NUM = 100;
    
    ITestScript* testScript;

    FILE* resultFilePointer;
};
