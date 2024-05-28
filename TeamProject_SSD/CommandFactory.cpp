#include "CommandFactory.h"

Command* CommandSingletonFactory::create(SSD* pSsd, std::queue<std::string> qCmdBuffer)
{
    AssertArguments(qCmdBuffer);

    if (sCmd == "write") {
        return new WriteCommand(pSsd, nLba, nData);
    }
    else if (sCmd == "read") {
        return new ReadCommand(pSsd, nLba);
    }
    else if (sCmd == "erase") {
        return new EraseCommand(pSsd, nLba, nSize);
    }
    else if (sCmd == "erase_range") {
        return new EraseRangeCommand(pSsd, nStartLba, nEndLba);
    }
    else if (sCmd == "flush") {
        return new FlushCommand(pSsd);
    }
    else if (sCmd == "exit") {
        return new ExitCommand(pSsd);
    }
    else if (sCmd == "help") {
        return new HelpCommand(pSsd);
    }
    else if (sCmd == "fullwrite") {
        return new FullWriteCommand(pSsd, nData);
    }
    else if (sCmd == "fullread") {
        return new FullReadCommand(pSsd);
    }
    else if (sCmd.find("testscript") != std::string::npos) {
        return new DoScriptCommand(pSsd, sCmd);
    }
    else if (sCmd.find(".lst") != std::string::npos) {
        return new RunListCommmand(pSsd, sCmd);
    }

    return new WrongCommand(pSsd);
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

        nLba = ConvertToNumFrom(qCmdBuffer.front());  qCmdBuffer.pop();
        nData = ConvertToNumFrom(qCmdBuffer.front());  qCmdBuffer.pop();
    }
    else if (sCmd == "read") {
        if (qCmdBuffer.size() != 1) 
            throw std::invalid_argument("Wrong arguments : read [LBA]");
        nLba = ConvertToNumFrom(qCmdBuffer.front());  qCmdBuffer.pop();
    }
    else if (sCmd == "fullwrite") {
        if (qCmdBuffer.size() != 1)
            throw std::invalid_argument("Wrong arguments : fullwrite [DATA]");
        nData = ConvertToNumFrom(qCmdBuffer.front());  qCmdBuffer.pop();
    }
    else if (sCmd == "erase") {
        if (qCmdBuffer.size() != 2)
            throw std::invalid_argument("Wrong arguments : erase [LBA] [SIZE]");
        nLba = ConvertToNumFrom(qCmdBuffer.front());  qCmdBuffer.pop();
        nSize = ConvertToNumFrom(qCmdBuffer.front());  qCmdBuffer.pop();
    }
    else if (sCmd == "erase_range") {
        if (qCmdBuffer.size() != 2)
            throw std::invalid_argument("Wrong arguments : erase_range [Start LBA] [End LBA]");
        nStartLba = ConvertToNumFrom(qCmdBuffer.front());  qCmdBuffer.pop();
        nEndLba = ConvertToNumFrom(qCmdBuffer.front()) - 1;  qCmdBuffer.pop();
    }
}

unsigned int CommandSingletonFactory::ConvertToNumFrom(std::string sNumber) {
    try {
        if (sNumber.find("0x") != std::string::npos)
            return stoul(sNumber, 0, 16);
        else
            return stoul(sNumber, 0, 10);
    }
    catch (std::exception& e) {
        throw e;
    }
    
}
