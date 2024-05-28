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

    // Methods for Runner
    void DoRunner(std::string& sCmd);
    void DoRunnerTestscenario(RunnerFileHandler& runnerFileHandler);
    bool DoFullWriteReadCompare();
    bool DoFullRead10AndCompare();
    bool DoWrite10AndCompare();
    bool DoLoop_WriteAndReadCompare();
    void PrintRunnerResult(bool isPassed);
private:    
    TestScript* testScript;
    SSD* pSsd;
    Command* pCommand;

    int GetSsdSize();

    void PrintBlockData(unsigned int nLba, unsigned int data);
};
