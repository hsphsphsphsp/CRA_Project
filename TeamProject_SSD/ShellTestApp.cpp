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
    Log.Print(__func__,  "******************************************\n");
    Log.Print(__func__,  "*           Shell Test Program           *\n");
    Log.Print(__func__,  "******************************************\n");
    Log.Print(__func__,  "*If you need any help, type \"help\"\n");

    while (1)
    {
        cout << "*> ";

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
            Log.Print(__func__,  "*> %s is done!\n", pCommand->sCmdName.c_str());
        }
        catch (std::exception& e) {
            Log.Print(__func__,  e.what());
            cout << endl;
            Log.Print(__func__,  "*> %s throw error !!!\n", pCommand->sCmdName.c_str());
        }
    }
}

int ShellTestApp::GetSsdSize() {
    return pSsd->GetSSDSize();
}

void ShellTestApp::PrintBlockData(unsigned int nLba, unsigned int nData) {
    Log.Print(__func__,  "LBA = %d\tDATA = %#x\n", nLba, nData);
}
