#include "ShellTestApp.h"


ShellTestApp::ShellTestApp(SSD* pSsd) : pSsd{ pSsd } {

}

void ShellTestApp::write(unsigned int nLba, unsigned int nData) {
    try {
        for (int c = 0; c < CYCLE; c++)
            pSsd->Write(nLba, nData);
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

void ShellTestApp::read(unsigned int nLba) {

}

void ShellTestApp::exit() {

}

void ShellTestApp::help() {

}

void ShellTestApp::fullWrite(unsigned int nData) {

}

void ShellTestApp::fullRead() {

}

void ShellTestApp::doScript(std::string sTestScriptName) {

}
