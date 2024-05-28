#include "ShellTestApp.h"
#include <sstream>
#include <queue>
#include "Runner.h"

ShellTestApp::ShellTestApp(SSD* pSsd) : pSsd{ pSsd } {

}

void ShellTestApp::Write(unsigned int nLba, unsigned int nData) {
    try {
        pSsd->Write(nLba, nData);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void ShellTestApp::Read(unsigned int nLba) {
    try {
        unsigned int nData = pSsd->Read(nLba);
        PrintBlockData(nLba, nData);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void ShellTestApp::Erase(unsigned int nLba, unsigned int nSize) {
    try {
        pSsd->Erase(nLba, nSize);
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void ShellTestApp::Exit() {
    std::string sInput;

    while (1) {
        std::cout << "Exit App? (yes, no)" << std::endl;
        std::cin >> sInput;

        if (sInput == "yes")
            std::exit(0);
        else if (sInput == "no")
            return;
        else
            std::cout << "Please type yes or no" << std::endl;
    }
}

void ShellTestApp::Help() {
    std::cout << "------------ Shell Test App ------------" << std::endl;

    std::cout << "* Specs ********************************" << std::endl;
    std::cout << "* Max number of LBA = " << dec << pSsd->GetSSDSize() << std::endl;

    std::cout<< std::endl;
    std::cout << "* Commands *****************************" << std::endl;
    
    std::cout << "- write [lba: number] [data: number]" << std::endl;
    std::cout << "\tWrite data to NAND at LBA number BLOCK" << std::endl;

    std::cout << "- fullwrite [data: number]" << std::endl;
    std::cout << "\tWrite data to all NAND BLOCK" << std::endl;

    std::cout << "- read [lba: number]" << std::endl;
    std::cout << "\tRead data from NAND at LBA number BLOCK" << std::endl;

    std::cout << "- fullread" << std::endl;
    std::cout << "\tRead all data in NAND and Print" << std::endl;

    std::cout << "- erase [lba: number] [size: number]" << std::endl;
    std::cout << "\tErase data as much as size from the LBA number BLOCK" << std::endl;

    std::cout << "- erase_range [start lba: number] [end lba: number]" << std::endl;
    std::cout << "\tErase data from start LBA number BLOCK to end LBA number BLOCK" << std::endl;

    std::cout << "- exit" << std::endl;
    std::cout << "\tExit this program" << std::endl;

    std::cout << "- testapp1" << std::endl;
    std::cout << "\tDo below Test Sequence" << std::endl;
    std::cout << "\tfullwrite > fullread" << std::endl;
    std::cout << "\tCheck is correctly writed" << std::endl;

    std::cout << "- testapp2" << std::endl;
    std::cout << "\tDo below Test Sequence" << std::endl;
    std::cout << "\tWrite 0xAAAABBBB to 0~5 LBA" << std::endl;
    std::cout << "\tOver Write 0x12345678 to 0~5 LBA" << std::endl;
    std::cout << "\tRead 0~5 LBA" << std::endl;
    std::cout << "\tCheck is 0~5 LBA data overwritted" << std::endl;
}

void ShellTestApp::FullWrite(unsigned int nData) {
    try {
        for (unsigned int nLba = 0; nLba < GetSsdSize(); nLba++)
            pSsd->Write(nLba, nData);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void ShellTestApp::FullRead() {
    try {
        for (unsigned int nLba = 0; nLba < GetSsdSize(); nLba++)
            PrintBlockData(nLba, pSsd->Read(nLba));
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void ShellTestApp::DoScript(std::string sTestScriptName) {
    testScript = TestScriptFactory::createScript(sTestScriptName, *pSsd);

    if (testScript == nullptr)
    {
        throw exception("INVALID SCRIPT NAME");
    }
    testScript->DoScript();
}

void ShellTestApp::Start()
{
    string sCmd;
    queue<string> qCmdBuffer;

    while (1)
    {
        string sLine;
        getline(cin, sLine);
        istringstream iSS(sLine);
        string sBuffer;

        while (getline(iSS, sBuffer, ' '))
        {
            qCmdBuffer.push(sBuffer);
        }

        string sCmd = qCmdBuffer.front();
        qCmdBuffer.pop();

        if (sCmd == "r")
        {
            if (qCmdBuffer.size() != 1)
                throw exception("INVALID CMD");

            unsigned int nLba = stoi(qCmdBuffer.front());
            qCmdBuffer.pop();
            Read(nLba);
        }
        else if (sCmd == "w")
        {
            unsigned int nLba = stoi(qCmdBuffer.front());
            qCmdBuffer.pop();
            unsigned int nValue = stoul(qCmdBuffer.front(), nullptr, 16);
            qCmdBuffer.pop();
            Write(nLba, nValue);
        }
        else if (sCmd == "erase")
        {
            if (qCmdBuffer.size() != 2)
                throw exception("INVALID CMD");

            unsigned int nLba = stoi(qCmdBuffer.front());
            qCmdBuffer.pop();
            unsigned int nSize = stoi(qCmdBuffer.front());
            qCmdBuffer.pop();
            Erase(nLba, nSize);
        }
        else if (sCmd == "erase_range")
        {
            if (qCmdBuffer.size() != 2)
                throw exception("INVALID CMD");

            unsigned int nStartLba = stoi(qCmdBuffer.front());
            qCmdBuffer.pop();
            unsigned int nEndLba = stoi(qCmdBuffer.front());
            qCmdBuffer.pop();
            Erase(nStartLba, nEndLba - nStartLba);
        }
        else if (sCmd == "exit")
        {
            return;
        }
        else if (sCmd == "help")
        {
            Help();
        }
        else if (sCmd == "fullwrite")
        {
            unsigned int nValue = stoi(qCmdBuffer.front());
            qCmdBuffer.pop();
            FullWrite(nValue);
        }
        else if (sCmd == "fullread")
        {
            FullRead();
        }
        else if (sCmd == "run_list.lst")
        {
            Runner runnerHandler(pSsd);
            if (!runnerHandler.CheckRunListFileExist(sCmd)) continue;
            runnerHandler.DoRunnerTestScenario();
        }
        else
        {
            // not defined cmd
        }
    }
}

int ShellTestApp::GetSsdSize() {
    return pSsd->GetSSDSize();
}

void ShellTestApp::PrintBlockData(unsigned int nLba, unsigned int nData) {
    std::cout << "LBA  = " << dec << nLba << "\t";
    std::cout << "DATA = 0x" << uppercase << hex << nData << std::endl;
}
