#include "CommandFactory.h"

Command* CommandSingletonFactory::create(std::queue<std::string> qCmdBuffer)
{
    AssertArguments(qCmdBuffer);

    if (sCmd == "write") {
        return new WriteCommand(nLba, nData);
    }
    else if (sCmd == "read") {
        return new ReadCommand(nLba);
    }
    else if (sCmd == "exit") {
        return new ExitCommand();
    }
    else if (sCmd == "help") {
        return new HelpCommand();
    }
    else if (sCmd == "fullwrite") {
        return new FullWriteCommand(nData);
    }
    else if (sCmd == "fullread") {
        return new FullReadCommand();
    }

    return new WrongCommand();
}

void CommandSingletonFactory::AssertArguments(std::queue<std::string> qCmdBuffer)
{
    if (qCmdBuffer.empty())
        throw std::invalid_argument("Please input command.");

    sCmd = qCmdBuffer.front();
    qCmdBuffer.pop();

    if (sCmd == "write") {
        if (qCmdBuffer.size() != 2)
            throw std::invalid_argument("Wrong arguments : write [LBA] [DATA]");

        nLba = stoi(qCmdBuffer.front());  qCmdBuffer.pop();
        nData = stoi(qCmdBuffer.front());  qCmdBuffer.pop();
    }
    else if (sCmd == "read") {
        if (qCmdBuffer.size() != 1) {
            throw std::invalid_argument("Wrong arguments : read [LBA]");
        }
        nLba = stoi(qCmdBuffer.front());  qCmdBuffer.pop();
    }
    else if (sCmd == "fullwrite") {
        if (qCmdBuffer.size() != 1) {
            throw std::invalid_argument("Wrong arguments : fullwrite [DATA]");
        }
        nData = stoi(qCmdBuffer.front());  qCmdBuffer.pop();
    }
}
