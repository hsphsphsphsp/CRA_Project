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
