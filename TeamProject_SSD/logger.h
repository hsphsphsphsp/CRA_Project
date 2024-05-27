#pragma once

#include "singleton.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <time.h>

using namespace std;

class Logger
{
	MAKESINGLE(Logger)

public:
	void CreateNewLog();
	void TransToZip();
	void Print(string sLog, string sFunctionName);
	string GetTime();
	int GetFileSize();

private:
	queue<string> qLogFiles;

	const string sLatestFileName = "latest.log";
};