#pragma once

#include <queue>
#include <string>
#include <iostream>
#include <Windows.h>

using namespace std;

class Logger
{
public:
	static Logger& getInstance() {
		static Logger instance{};
		return instance;
	}
	void CreateNewLog();
	void TransFileToZip();
	void RenameFile(std::string& sPathAndOldFileName, std::string& sNewFileName);
	void Logging(string sLog);
	string ExtractFileName(const string& sFileName);

private:
	queue<string> qLogFiles;
	const string sLatestFileName = "latest.log";
	string sLogFolderPath;

	Logger();
	Logger& operator=(const Logger& other) = delete;
	Logger(const Logger& other) = delete;
};