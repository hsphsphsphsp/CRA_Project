#include "Command.h"

Command::Command(SSD* pSsd) :
	pSsd{pSsd}
{
}

void Command::PrintBlockData(unsigned int nLba, unsigned int nData) {
	std::cout << "LBA  = " << dec << nLba << "\t";
	std::cout << "DATA = 0x" << uppercase << hex << nData << std::endl;
}


WriteCommand::WriteCommand(SSD* pSsd, int nLba, int nData) :
	Command(pSsd),
	nLba{nLba},
	nData{nData}
{
}

void WriteCommand::execute()
{
	try {
		pSsd->Write(nLba, nData);
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

ReadCommand::ReadCommand(SSD* pSsd, int nLba) :
	Command(pSsd),
	nLba{ nLba }
{
}

void ReadCommand::execute()
{
	try {
		unsigned int nData = pSsd->Read(nLba);
		PrintBlockData(nLba, nData);
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

ExitCommand::ExitCommand(SSD* pSsd) :
	Command(pSsd)
{
}

void ExitCommand::execute()
{
	std::string sInput;

	while (1) {
		std::cout << "Exit App? (yes, no)" << std::endl;
		std::cin >> sInput;

		if (sInput == "yes")
			std::exit(0);
		else if (sInput == "no")
			return;
		else
			std::cout << "Please type yes or no" << std::endl;
	}
}

HelpCommand::HelpCommand(SSD* pSsd) : 
	Command(pSsd)
{
}

void HelpCommand::execute()
{
	std::cout << "------------ Shell Test App ------------" << std::endl;

	std::cout << "* Specs ********************************" << std::endl;
	std::cout << "* Max number of LBA = " << dec << pSsd->GetSSDSize() << std::endl;

	std::cout << std::endl;
	std::cout << "* Commands *****************************" << std::endl;

	std::cout << "- write [lba: number] [data: number]" << std::endl;
	std::cout << "\tWrite data to NAND at LBA number BLOCK" << std::endl;

	std::cout << "- fullwrite [data: number]" << std::endl;
	std::cout << "\tWrite data to all NAND BLOCK" << std::endl;

	std::cout << "- read [lba: number]" << std::endl;
	std::cout << "\tRead data from NAND at LBA number BLOCK" << std::endl;

	std::cout << "- fullread" << std::endl;
	std::cout << "\tRead all data in NAND and Print" << std::endl;

	std::cout << "- exit" << std::endl;
	std::cout << "\tExit this program" << std::endl;

	std::cout << "- testscriptapp1" << std::endl;
	std::cout << "\tDo below Test Sequence" << std::endl;
	std::cout << "\tfullwrite > fullread" << std::endl;
	std::cout << "\tCheck is correctly writed" << std::endl;

	std::cout << "- testscriptapp1" << std::endl;
	std::cout << "\tDo below Test Sequence" << std::endl;
	std::cout << "\tWrite 0xAAAABBBB to 0~5 LBA" << std::endl;
	std::cout << "\tOver Write 0x12345678 to 0~5 LBA" << std::endl;
	std::cout << "\tRead 0~5 LBA" << std::endl;
	std::cout << "\tCheck is 0~5 LBA data overwritted" << std::endl;
}

FullWriteCommand::FullWriteCommand(SSD* pSsd, int nData) :
	Command(pSsd),
	nData{nData}
{
}

void FullWriteCommand::execute()
{
	try {
		for (unsigned int nLba = 0; nLba < pSsd->GetSSDSize(); nLba++)
			pSsd->Write(nLba, nData);
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

FullReadCommand::FullReadCommand(SSD* pSsd) :
	Command(pSsd)
{
}

void FullReadCommand::execute()
{
	try {
		for (unsigned int nLba = 0; nLba < pSsd->GetSSDSize(); nLba++)
			PrintBlockData(nLba, pSsd->Read(nLba));
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

DoScriptCommand::DoScriptCommand(SSD* pSsd, std::string sTestScriptName):
	Command(pSsd),
	sTestScriptName{sTestScriptName}
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
	Command(pSsd)
{
}

void WrongCommand::execute()
{
	std::cout << "Wrong command, if you need to help, type \"help\"" << std::endl;
}

