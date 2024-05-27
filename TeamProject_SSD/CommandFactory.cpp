#include "CommandFactory.h"

Command* CommandFactory::create(std::queue<std::string> qCmdBuffer)
{
    if (qCmdBuffer.empty())
        return nullptr;

    std::string sCmd = qCmdBuffer.front();
    qCmdBuffer.pop();

    if (sCmd == "write") {
        if (qCmdBuffer.size() != 2) {
            std::cout << "Wrong arguments : write [LBA] [DATA]" << std::endl;
            return nullptr;
        }

        int nLba = stoi(qCmdBuffer.front());  qCmdBuffer.pop();
        int nData = stoi(qCmdBuffer.front());  qCmdBuffer.pop();

        return new WriteCommand(nLba, nData);
    }
    else if (sCmd == "read") {
        if (qCmdBuffer.size() != 1) {
            std::cout << "Wrong arguments : read [LBA]" << std::endl;
            return nullptr;
        }
        int nLba = stoi(qCmdBuffer.front());  qCmdBuffer.pop();

        return new ReadCommand(nLba);
    }
    else if (sCmd == "exit") {
        return new ExitCommand();
    }
    else if (sCmd == "help") {
        return new HelpCommand();
    }
    else if (sCmd == "fullwrite") {
        if (qCmdBuffer.size() != 1) {
            std::cout << "Wrong arguments : fullwrite [DATA]" << std::endl;
            return nullptr;
        }

        int nData = stoi(qCmdBuffer.front());  qCmdBuffer.pop();

        return new FullWriteCommand(nData);
    }
    else if (sCmd == "fullread") {
        return new FullReadCommand();
    }
    
    return new WrongCommand();
}
