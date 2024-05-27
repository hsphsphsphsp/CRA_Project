#pragma once

#include <queue>
#include <string>
#include <iostream>
#include <Windows.h>
#include <fstream>

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
	void Print(string sLog, string sFunctionName = __builtin_FUNCTION());
	void RenameFile(std::string& sPathAndOldFileName, std::string& sNewFileName);
	string ExtractFileName(const string& sFileName);
	string GetTime();
	int GetFileSize();

private:
	queue<string> qLogFiles;
	const string sLatestFileName = "latest.log";
	string sLogFolderPath;

	Logger();
	Logger& operator=(const Logger& other) = delete;
	Logger(const Logger& other) = delete;
};