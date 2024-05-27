#pragma once
#include <cstdio>
#include <iostream>
#include <string>

#include "testscript.h"
#include "CommandFactory.h"

class Command;

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

    void Start();

private:    
    TestScript* testScript;
    SSD* pSsd;

    Command* pCommand;

    int GetSsdSize();

    void PrintBlockData(unsigned int nLba, unsigned int data);
};
