#pragma once

#include <string>

#include "ITestScript.h"

class TestScriptFactory {
public:
    ITestScript* getTestScript(std::string sTestScriptName);
};