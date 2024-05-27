#include "logger.h"
#define DEFINE_10KB (10*1024)

void Logger::Print(string sLog, std::string sFunctionName = __builtin_FUNCTION())
{
	string result;
	ofstream fout(sLatestFileName);
	result += GetTime();
	result += sFunctionName;
	result += sLog;
	if (this->GetFileSize() + result.size() > DEFINE_10KB)
	{
		CreateNewLogFile();
	}
	
	cout << result << endl; 
	fout << result << endl;
	fout.close();
	return;
}
string Logger::GetTime()
{
	char result[100];
	time_t  timer;
	struct tm t;
	timer = time(NULL);
	localtime_s(&t, &timer);
	sprintf_s(result, sizeof(result), "[%d.%2d.%d %d:%d]", t.tm_year - 100, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min);
	return string(result);
}
int Logger::GetFileSize()
{
	return 0;
}
void Logger::CreateNewLog()
{
	time_t  timer;
	struct tm t;
	timer = time(NULL);
	localtime_s(&t, &timer);

	string sOldFileName;
	string sNewFileName;
	sOldFileName = "log\\latest.log";
	sNewFileName += "log\\until";
	sprintf_s((char*)sNewFileName.c_str(), sizeof(sNewFileName), "%d%2d%2d_%h_%dm_%ds.log", t.tm_year - 100, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min);
	if (rename((char*)sOldFileName.c_str(), (char*)sNewFileName.c_str()) != 0)
	{
		cout << "rename failed" << endl;
		return;
	}
	// enqueue sNewFileName;
}