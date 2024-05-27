#include "RunnerHandler.h"

bool RunnerHandler::Runner()
{
	if (!CheckRunnerFileExist()) return false;
	MakeCommandList();
	return DoTestScenario();
}

void RunnerHandler::MakeCommandList()
{
	commandList = runnerFileHandler.GetCommandListFromTheRunnerFile();
}

bool RunnerHandler::CheckRunnerFileExist()
{
	if (!runnerFileHandler.IsRunnerListFileExist())
	{
		cout << "Runner File does not exist." << endl;
		return false;
	}
	return true;
}

bool RunnerHandler::DoTestScenario()
{
	for (auto& command : commandList)
	{
		PrintRunMessageBeforeTest(command);
		if (command == "FullWriteReadCompare")
		{
			if (!DoFullWriteReadCompare()) return false;
		}
		if (command == "FullRead10AndCompare")
		{

		}
		if (command == "Write10AndCompare")
		{
			if (!DoWrite10AndCompare()) return false;
		}
		if (command == "Loop_WriteAndReadCompare")
		{
			// TBD
		}
	}
	cout << "Pass" << endl;
	return true;
}

bool RunnerHandler::DoWrite10AndCompare()
{
	unsigned int nWriteData = 0x12345678;
	unsigned int nTargetAddr = 0x0;
	int nTestEndCount = 10;

	for (int testCount = 0; testCount < nTestEndCount; testCount++)
	{
		pShellTestApp->Write(nTargetAddr, nWriteData);
		if (nWriteData != pShellTestApp->Read(nTargetAddr))
		{
			cout << "FAIL...!" << endl;
			return false;
		}
	}
	return true;
}

void RunnerHandler::PrintRunMessageBeforeTest(std::string& command)
{
	cout << command << "   ---   Run...";
}

bool RunnerHandler::DoFullWriteReadCompare()
{
	unsigned int nRefDataForFullWirteReadCompare = 0x123456678;
	pShellTestApp->FullWrite(nRefDataForFullWirteReadCompare);
	for (register int nandFileOffset = 0; nandFileOffset < pShellTestApp->GetSsdSize(); nandFileOffset)
	{
		if (pShellTestApp->Read(nandFileOffset) != nRefDataForFullWirteReadCompare)
		{
			cout << "FAIL...!" << endl;
			return false;
		}
	}
	return true;
}
