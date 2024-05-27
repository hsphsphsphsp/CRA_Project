#pragma once
#include <cstdio>
#include <iostream>
#include <string>

#include "testscript.h"

class ShellTestApp {
public:
    ShellTestApp(SSD* pSsd);
    
    void Write(unsigned int nLba, unsigned int nData);

    unsigned int Read(unsigned int nLba);

    void Exit();

    void Help();

    void FullWrite(unsigned int nData);

    void FullRead();

    void DoScript(std::string sTestScriptName);

    void Start();

    int GetSsdSize();
private:    
    TestScript* testScript;
    SSD* pSsd;

    void PrintBlockData(unsigned int nLba, unsigned int data);
};
