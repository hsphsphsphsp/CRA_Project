#include "RunnerFileHandler.h"

vector<string>& RunnerFileHandler::GetCommandListFromTheRunnerFile()
{
	string sIndex, sValue;
	if (IsRunnerListFileExist())
	{
		ifstream fin(sRunListFileName);
		while (!fin.eof())
		{
			fin >> sIndex >> sValue;
			commandList.push_back(sIndex);
		}
	}
	return commandList;
}

bool RunnerFileHandler::IsRunnerListFileExist()
{
	return _access(&*sRunListFileName.begin(), 0) == 0;
}