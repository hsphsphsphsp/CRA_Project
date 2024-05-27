#include "RunnerHandler.h"

bool RunnerHandler::Runner()
{
	if (!runnerFileHandler.IsRunnerListFileExist())
	{
		cout << "Runner File does not exist." << endl;
		return false;
	}
	commandList = runnerFileHandler.GetCommandListFromTheRunnerFile();

	return DoTestScenario();
}

bool RunnerHandler::DoTestScenario()
{
	for (auto& command : commandList)
	{
		if (command == "FullWriteReadCompare")
		{
			unsigned int nRefDataForFullWirteReadCompare = 0x123456678;
			shellTestApp->FullWrite(nRefDataForFullWirteReadCompare);
			for (register int nandFileOffset = 0; nandFileOffset < shellTestApp->GetSsdSize(); nandFileOffset)
			{
				if (shellTestApp->Read(nandFileOffset) != nRefDataForFullWirteReadCompare)
				{
					return false;
				}
			}
		}
		if (command == "FullRead10AndCompare")
		{
			// TBD
		}
		if (command == "Write10AndCompare")
		{
			// TBD
		}
		if (command == "Loop_WriteAndReadCompare")
		{
			// TBD
		}
	}
	return true;
}