#pragma once

#include "singleton.h"
#include <queue>
#include <string>

using namespace std;

class Logger
{
	MAKESINGLE(Logger)

public:
	void CreateNewLog();
	void TransToZip();
	void Logging(string sLog);

private:
	queue<string> qLogFiles;

	const string sLatestFileName = "latest.log";
};