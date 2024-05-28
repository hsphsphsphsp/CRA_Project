#include "ShellTestApp.h"
#include <sstream>
#include <queue>
#include "RunnerHandler.h"

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

int ShellTestApp::GetSsdSize() {
    return pSsd->GetSSDSize();
}

void ShellTestApp::PrintBlockData(unsigned int nLba, unsigned int nData) {
    std::cout << "LBA  = " << dec << nLba << "\t";
    std::cout << "DATA = 0x" << uppercase << hex << nData << std::endl;
}
