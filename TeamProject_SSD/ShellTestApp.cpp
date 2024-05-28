#include "ShellTestApp.h"
#include <sstream>
#include <queue>

ShellTestApp::ShellTestApp(SSD* pSsd) : pSsd{ pSsd } {

}

void ShellTestApp::Start()
{
    queue<string> qCmdBuffer;

    std::cout << std::endl;
    std::cout << "******************************************" << std::endl;
    std::cout << "*           Shell Test Program           *" << std::endl;
    std::cout << "******************************************" << std::endl;
    std::cout << "*If you need any help, type \"help\"" << std::endl;

    while (1)
    {
        std::cout << std::endl;
        std::cout << "*> ";

        qCmdBuffer = queue<string>();

        string sLine;
        getline(cin, sLine);
        istringstream iSS(sLine);
        string sBuffer;

        system("cls");
        
        while (getline(iSS, sBuffer, ' '))
        {
            qCmdBuffer.push(sBuffer);
        }
        try {
            pCommand = CommandSingletonFactory::GetInstance()
                .create(pSsd, qCmdBuffer);
            pCommand->execute();

            std::cout << "*> " << pCommand->sCmdName << " is done!" << std::endl;
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            std::cout << "*> " << pCommand->sCmdName << " throw error !!!" << std::endl;
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
