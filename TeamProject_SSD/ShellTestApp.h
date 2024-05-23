#pragma once
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

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
    TestScript* testScript;
    SSD* pSsd;

    int GetSsdSize();

    void PrintBlockData(unsigned int nLba, unsigned int data);
};
