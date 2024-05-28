#include "Runner.h"
#include <iostream>

Runner::Runner(SSD* pSsd) : pSsd(pSsd)
{
}

bool Runner::CheckRunListFileExist(std::string& sCmd)
{
    if (!runnerFileHandler.IsRunnerListFileExist(sCmd))
    {
        Log.Print(__func__,  "%s file does not exist.\n", sCmd.c_str());
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
    Log.Print(__func__,  "%s   ---   Run...", EachCommand.c_str());

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