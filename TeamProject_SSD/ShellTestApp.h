#pragma once
#include <cstdio>
#include <iostream>
#include <string>

#include "testscript.h"
#include "RunnerFileHandler.h"
#include "CommandFactory.h"
#include "Command.h"

class ShellTestApp {
public:
    ShellTestApp(SSD* pSsd);
    
    void Start();

private:    
    TestScript* testScript;
    SSD* pSsd;
    Command* pCommand;

    int GetSsdSize();

    void PrintBlockData(unsigned int nLba, unsigned int data);
};
