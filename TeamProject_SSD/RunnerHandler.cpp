#include "RunnerHandler.h"
#include <iostream>

RunnerHandler::RunnerHandler(SSD* pSsd) : pSsd(pSsd)
{
}

bool RunnerHandler::CheckRunListFileExist(std::string& sCmd)
{
    if (!runnerFileHandler.IsRunnerListFileExist(sCmd))
    {
        std::cout << sCmd << " file does not exist." << endl;
        return false;
    }
    return true;
}

void RunnerHandler::DoRunnerTestScenario()
{
    vector<string> vRunnerCmdList = runnerFileHandler.GetCommandListFromTheRunnerFile();

    for (auto& EachCommand : vRunnerCmdList)
    {
        TestScript* pTestScript;
        pTestScript = PrepareForEachTestScenario(EachCommand);
        PrintRunnerResult(StartEachScenarioTest(pTestScript));
    }
}

TestScript* RunnerHandler::PrepareForEachTestScenario(std::string& EachCommand)
{
    cout << EachCommand << "   ---   Run...";

    TestScriptFactory fTestScriptFactory;
    return fTestScriptFactory.createScript(EachCommand, *pSsd);
}

bool RunnerHandler::StartEachScenarioTest(TestScript* pTestScript)
{
    if (pTestScript == nullptr) return false;
    return pTestScript->DoScript();
}

void RunnerHandler::PrintRunnerResult(bool isPassed)
{
    if (isPassed)
        cout << "Pass" << endl;
    else
        cout << "Fail!" << endl;
}