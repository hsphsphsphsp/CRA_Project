#include "ShellTestApp.h"
#include <sstream>
#include <queue>
#include "Runner.h"

ShellTestApp::ShellTestApp(SSD* pSsd) : pSsd{ pSsd } {

}

void ShellTestApp::Start()
{
    queue<string> qCmdBuffer;

    Log.Print(__func__, "\n");
    Log.Print(__func__,  "******************************************\n");
    Log.Print(__func__,  "*           Shell Test Program           *\n");
    Log.Print(__func__,  "******************************************\n");
    Log.Print(__func__,  "*If you need any help, type \"help\"\n");

    while (1)
    {
        Log.Print(__func__, "*> ");

        qCmdBuffer = queue<string>();

        string sLine;
        getline(cin, sLine);
        Log.Print("", sLine + "\n");
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
            Log.Print(__func__,  string(e.what()) + "\n");
        }
    }
}
