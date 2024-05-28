#include "ShellTestApp.h"
#include <sstream>
#include <queue>

ShellTestApp::ShellTestApp(SSD* pSsd) : pSsd{ pSsd } {

}

void ShellTestApp::Start()
{
    queue<string> qCmdBuffer;

    while (1)
    {
        qCmdBuffer = queue<string>();

        string sLine;
        getline(cin, sLine);
        istringstream iSS(sLine);
        string sBuffer;

        while (getline(iSS, sBuffer, ' '))
        {
            qCmdBuffer.push(sBuffer);
        }
        try {
            pCommand = CommandSingletonFactory::GetInstance()
                .create(pSsd, qCmdBuffer);
            pCommand->execute();
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
}

void ShellTestApp::DoRunner(std::string& sCmd)
{
    RunnerFileHandler runnerFileHandler;
    if (!runnerFileHandler.IsRunnerListFileExist(sCmd))
    {
        cout << sCmd << " file does not exist." << endl;
        return;
    }

    DoRunnerTestscenario(runnerFileHandler);
}

void ShellTestApp::DoRunnerTestscenario(RunnerFileHandler& runnerFileHandler)
{
    vector<string> vRunnerCmdList = runnerFileHandler.GetCommandListFromTheRunnerFile();

    for (auto& command : vRunnerCmdList)
    {
        bool bIsPassed = true;
        cout << command << "   ---   Run...";

        TestScriptFactory fTestScriptFactory;
        TestScript* pTestScript = fTestScriptFactory.createScript(command, *pSsd);

        if (pTestScript == nullptr)
        {
            bIsPassed = false;
        }
        else
        {
            bIsPassed = pTestScript->DoScript();
        }

        PrintRunnerResult(bIsPassed);
    }
}

void ShellTestApp::PrintRunnerResult(bool isPassed)
{
    if (isPassed)
        cout << "Pass" << endl;
    else
        cout << "Fail!" << endl;
}

int ShellTestApp::GetSsdSize() {
    return pSsd->GetSSDSize();
}

void ShellTestApp::PrintBlockData(unsigned int nLba, unsigned int nData) {
    std::cout << "LBA  = " << dec << nLba << "\t";
    std::cout << "DATA = 0x" << uppercase << hex << nData << std::endl;
}
