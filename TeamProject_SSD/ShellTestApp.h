#pragma once
#include <cstdio>
#include <iostream>
#include <string>

#include "testscript.h"

class ShellTestApp {
public:
    ShellTestApp(SSD* pSsd);
    
    void Write(unsigned int nLba, unsigned int nData);

    void Read(unsigned int nLba);

    void Exit();

    void Help();

    void FullWrite(unsigned int nData);

    void FullRead();

    void DoScript(std::string sTestScriptName);

private:
    const unsigned int MAX_LBA_NUM = 100;
    
    TestScript* testScript;
    SSD* pSsd;

    FILE* fpResult;
};
