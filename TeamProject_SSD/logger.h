#pragma once

#include <queue>
#include <string>
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <sys/timeb.h>​
#include <cstdarg>

using namespace std;

#define DEFINE_10KB (10*1024)

class Logger
{
public:
	static Logger& getInstance() {
		static Logger instance{};
		return instance;
	}
	void CreateNewLog();
	void TransFileToZip();
	void Print(string sFunctionName, string format, ...);
	void RenameFile(string& sPathAndOldFileName, string& sNewFileName);
	string ExtractFileName(const string& sFileName);
	string GetTime();
	int GetFileSize();

private:
	queue<string> qLogFiles;
	const string sLatestFileName = "latest.log";
	string sLogFolderPath;

	Logger();
	Logger& operator=(const Logger& other) = delete;
};

extern Logger Log;