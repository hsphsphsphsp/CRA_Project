#include "Runner.h"
#include <iostream>

Runner::Runner(SSD* pSsd) : pSsd(pSsd)
{
}

bool Runner::CheckRunListFileExist(std::string& sCmd)
{
    if (!runnerFileHandler.IsRunnerListFileExist(sCmd))
    {
        std::cout << sCmd << " file does not exist." << endl;
        return false;
    }
    return true;
}

void Runner::DoRunnerTestScenario()
{
    vector<string> vRunnerCmdList = runnerFileHandler.GetCommandListFromTheRunnerFile();

    for (auto& EachCommand : vRunnerCmdList)
    {
        TestScript* pTestScript;
        pTestScript = PrepareForEachTestScenario(EachCommand);
        PrintRunnerResult(StartEachScenarioTest(pTestScript));
    }
}

TestScript* Runner::PrepareForEachTestScenario(std::string& EachCommand)
{
    cout << EachCommand << "   ---   Run...";

    TestScriptFactory fTestScriptFactory;
    return fTestScriptFactory.createScript(EachCommand, *pSsd);
}

bool Runner::StartEachScenarioTest(TestScript* pTestScript)
{
    if (pTestScript == nullptr) return false;
    return pTestScript->DoScript();
}

void Runner::PrintRunnerResult(bool isPassed)
{
    if (isPassed)
        cout << "Pass" << endl;
    else
        cout << "Fail!" << endl;
}