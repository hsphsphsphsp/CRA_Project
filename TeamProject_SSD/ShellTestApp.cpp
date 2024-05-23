#include "ShellTestApp.h"


ShellTestApp::ShellTestApp(SSD* pSsd) : pSsd{ pSsd } {

}

void ShellTestApp::Write(unsigned int nLba, unsigned int nData) {
    try {
        for (int c = 0; c < CYCLE; c++)
            pSsd->Write(nLba, nData);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void ShellTestApp::Read(unsigned int nLba) {

}

void ShellTestApp::Exit() {
    
}

void ShellTestApp::Help() {

}

void ShellTestApp::FullWrite(unsigned int nData) {

}

void ShellTestApp::FullRead() {

}

void ShellTestApp::DoScript(std::string sTestScriptName) {

}
