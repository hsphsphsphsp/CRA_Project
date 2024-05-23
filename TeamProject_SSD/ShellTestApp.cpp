#include "ShellTestApp.h"
#include <sstream>
#include <queue>

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

void ShellTestApp::Exit() {
    
}

void ShellTestApp::Help() {

}

void ShellTestApp::FullWrite(unsigned int nData) {

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
