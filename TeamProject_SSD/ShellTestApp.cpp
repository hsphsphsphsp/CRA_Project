#include "ShellTestApp.h"
#include <sstream>
#include <queue>
#include "Runner.h"

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
