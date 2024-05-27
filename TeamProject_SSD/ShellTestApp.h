#pragma once
#include <cstdio>
#include <iostream>
#include <string>

#include "testscript.h"
#include "RunnerFileHandler.h"

class ShellTestApp {
public:
    ShellTestApp(SSD* pSsd);
    
    void Write(unsigned int nLba, unsigned int nData);

    void Read(unsigned int nLba);

    void Erase(unsigned int nLba, unsigned int nSize);

    void Exit();

    void Help();

    void FullWrite(unsigned int nData);

    void FullRead();

    void DoScript(std::string sTestScriptName);

    void Start();

private:    
    TestScript* testScript;
    SSD* pSsd;

    int GetSsdSize();

    void PrintBlockData(unsigned int nLba, unsigned int data);
};
