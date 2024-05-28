#include "Command.h"

Command::Command(SSD* pSsd, std::string sCmdName) :
    pSsd{ pSsd },
    sCmdName{ sCmdName }
{
}

void Command::PrintBlockData(unsigned int nLba, unsigned int nData) {
    Log.Print(__func__, "LBA = %d \t DATA = %#x\n", nLba, nData);
}

WriteCommand::WriteCommand(SSD* pSsd, int nLba, int nData) :
    Command(pSsd, "Write"),
    nLba{ nLba },
    nData{ nData }
{
}

void WriteCommand::execute()
{
    pSsd->Write(nLba, nData);
}

ReadCommand::ReadCommand(SSD* pSsd, int nLba) :
    Command(pSsd, "Read"),
    nLba{ nLba }
{
}

void ReadCommand::execute()
{

    unsigned int nData = pSsd->Read(nLba);
    PrintBlockData(nLba, nData);

}

ExitCommand::ExitCommand(SSD* pSsd) :
    Command(pSsd, "Exit")
{
}

void ExitCommand::execute()
{
    std::string sInput;

    while (1) {
        Log.Print(__func__, "Exit App? (yes, no) : ");
        std::cin >> sInput;

        if (sInput == "yes") {
            pSsd->Flush();
            std::exit(0);
        }
        else if (sInput == "no") {
            return;
        }
        else {
            Log.Print(__func__, "Please type yes or no\n");
        }
    }
}

HelpCommand::HelpCommand(SSD* pSsd) :
    Command(pSsd, "Help")
{
}

void HelpCommand::execute()
{
    Log.Print(__func__, "\n");
    Log.Print(__func__,  "******************************************\n");
    Log.Print(__func__,  "*           Shell Test Program           *\n");
    Log.Print(__func__,  "******************************************\n");
    Log.Print(__func__,  "* Specs **********************************\n");
    Log.Print(__func__,  "* Max number of LBA = %d\n", pSsd->GetSSDSize());
    Log.Print(__func__, "* Commands *******************************\n");

    Log.Print(__func__, "- write [lba: number] [data: number]\n");
    Log.Print(__func__, "\tWrite data to NAND at LBA number BLOCK\n");
    Log.Print(__func__, "- fullwrite [data: number]\n");
    Log.Print(__func__, "\tWrite data to all NAND BLOCK\n");
    Log.Print(__func__, "- read [lba: number]\n");
    Log.Print(__func__, "\tRead data from NAND at LBA number BLOCK\n");
    Log.Print(__func__, "- fullread\n");
    Log.Print(__func__, "\tRead all data in NAND and Print\n");
    Log.Print(__func__, "- erase [lba: number] [size: number]\n");
    Log.Print(__func__, "\tErase data as much as size from the LBA number BLOCK\n");
    Log.Print(__func__, "- erase_range [start lba: number] [end lba: number]\n");
    Log.Print(__func__, "\tErase data from start LBA number BLOCK to end LBA number BLOCK\n");
    Log.Print(__func__, "- flush\n");
    Log.Print(__func__, "\tFlush the command buffer\n");
    Log.Print(__func__, "- exit\n");
    Log.Print(__func__, "\tExit this program\n");
    Log.Print(__func__, "- testscriptapp1\n");
    Log.Print(__func__, "\tDo below Test Sequence\n");
    Log.Print(__func__, "\tfullwrite > fullread\n");
    Log.Print(__func__, "\tCheck is correctly writed\n");
    Log.Print(__func__, "- testscriptapp2\n");
    Log.Print(__func__, "\tDo below Test Sequence\n");
    Log.Print(__func__, "\tWrite 0xAAAABBBB to 0~5 LBA\n");
    Log.Print(__func__, "\tOver Write 0x12345678 to 0~5 LBA\n");
    Log.Print(__func__, "\tRead 0~5 LBA\n");
    Log.Print(__func__, "\tCheck is 0~5 LBA data overwritted\n");
    Log.Print(__func__, "- [run list filename: string].lst\n");
    Log.Print(__func__, "\tRun all scripts in list file\n");
}

FullWriteCommand::FullWriteCommand(SSD* pSsd, int nData) :
    Command(pSsd, "Full Write"),
    nData{ nData }
{
}

void FullWriteCommand::execute()
{
    for (unsigned int nLba = 0; nLba < pSsd->GetSSDSize(); nLba++)
        pSsd->Write(nLba, nData);
}

FullReadCommand::FullReadCommand(SSD* pSsd) :
    Command(pSsd, "Full Read")
{
}

void FullReadCommand::execute()
{
    for (unsigned int nLba = 0; nLba < pSsd->GetSSDSize(); nLba++)
        PrintBlockData(nLba, pSsd->Read(nLba));
}

DoScriptCommand::DoScriptCommand(SSD* pSsd, std::string sTestScriptName) :
    Command(pSsd, "Script"),
    sTestScriptName{ sTestScriptName }
{
}

void DoScriptCommand::execute()
{
    TestScript* testScript = TestScriptFactory::createScript(sTestScriptName, *pSsd);

    if (testScript == nullptr)
    {
        throw exception("INVALID SCRIPT NAME");
    }
    testScript->DoScript();
}

WrongCommand::WrongCommand(SSD* pSsd) :
    Command(pSsd, "Unknown Command")
{
}

void WrongCommand::execute()
{
    Log.Print(__func__, "Wrong command, if you need to help, type \"help\"\n");
    throw std::exception("Unknown command...");
}

EraseCommand::EraseCommand(SSD* pSsd, int nLba, int nSize) :
    Command(pSsd, "Erase"),
    nLba{ nLba },
    nSize{ nSize }
{
}

void EraseCommand::execute()
{
    pSsd->Erase(nLba, nSize);
}

FlushCommand::FlushCommand(SSD* pSsd) :
    Command(pSsd, "Flush")
{
}

void FlushCommand::execute()
{
    pSsd->Flush();
}

RunListCommmand::RunListCommmand(SSD* pSsd, std::string sFileName) :
    Command(pSsd, "Run List"),
    sFileName{sFileName}
{
}

void RunListCommmand::execute()
{
    Runner runner(pSsd);

    if (!runner.CheckRunListFileExist(sFileName))
        throw std::exception("List file is not exist");
    runner.DoRunnerTestScenario();
}

EraseRangeCommand::EraseRangeCommand(SSD* pSsd, unsigned int nStartLba, unsigned int nEndLba) :
    Command(pSsd, "Erase Range"),
    nStartLba{ nStartLba },
    nEndLba{ nEndLba }
{
}

void EraseRangeCommand::execute()
{
    const unsigned int MAXSIZE = 10;
    unsigned int nSize;
    
    do {
        nSize = nEndLba - nStartLba + 1;
        if (nSize > MAXSIZE)
            nSize = MAXSIZE;
        else if (nSize <= 0)
            break;

        pSsd->Erase(nStartLba, nSize);
        nStartLba += nSize;
    } while (1);
}

