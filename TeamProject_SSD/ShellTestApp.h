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

    void Start();

private:    
    TestScript* testScript;
    SSD* pSsd;

    Command* pCommand;
};
