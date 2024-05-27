#pragma once
#include <io.h>
#include <string>
#include <fstream>
#include <iomanip>
#include <unordered_map>

using namespace std;

class RunnerFileHandler
{
public:
	vector<string>& GetCommandListFromTheRunnerFile();
	bool IsRunnerListFileExist(string fileName);

private:
	void MakeFilePath(string fileName);

	vector<string> commandList;
	string runnerFileName = "";
};
