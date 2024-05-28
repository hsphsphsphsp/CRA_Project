#pragma once
#include <cstdio>
#include <iostream>
#include <string>

#include "testscript.h"
#include "CommandFactory.h"
#include "Command.h"

class ShellTestApp {
public:
    ShellTestApp(SSD* pSsd);
    
    void Start();

private:    
    SSD* pSsd;
    Command* pCommand;
};
