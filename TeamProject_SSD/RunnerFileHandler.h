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
	vector<string>& GetCommandListFromTheRunnerFile(void);
	bool IsRunnerListFileExist();

private:
	const string sResultFileName = "result.txt";
	const string sRunListFileName = "run_list.lst.txt";
	vector<string> commandList;
};
