#include "ShellTestApp.h"


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
int ShellTestApp::GetSsdSize() {
    return pSsd->GetSSDSize();
}

void ShellTestApp::PrintBlockData(unsigned int nLba, unsigned int nData) {
    std::cout << "LBA  = " << dec << nLba << "\t";
    std::cout << "DATA = 0x" << uppercase << hex << nData << std::endl;
}
