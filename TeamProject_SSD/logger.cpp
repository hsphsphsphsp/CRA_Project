#include "logger.h"


Logger Log = Logger::getInstance();

string GetExeFilePath()
{
    wchar_t wExeFilePath[MAX_PATH];
    GetModuleFileName(NULL, wExeFilePath, MAX_PATH);

    wstring wsExeFilePath(wExeFilePath);
    string sExeFilePath(wsExeFilePath.begin(), wsExeFilePath.end());
    size_t nLastLocPos = sExeFilePath.find_last_of('\\');

    return sExeFilePath.substr(0, nLastLocPos);
}

void Logger::TransFileToZip()
{
    string sZipFileName = ExtractFileName(qLogFiles.front()) + ".zip";
    string sPathAndOldFileName = sLogFolderPath + qLogFiles.front();

    RenameFile(sPathAndOldFileName, sZipFileName);

    qLogFiles.pop();
}

void Logger::RenameFile(string& sPathAndOldFileName, string& sNewFileName)
{
    string command = "ren " + sPathAndOldFileName + " " + sNewFileName;

    int result = system(command.c_str());

    if (result != 0) {
        string sExceptionInfo = "Fail to rename " + sPathAndOldFileName + " -> " + sNewFileName;
        throw exception(sExceptionInfo.c_str());
    }
}

string Logger::ExtractFileName(const string& sFileName)
{
    size_t nLastDotPos = sFileName.find_last_of('.');

    if (nLastDotPos != string::npos) {
        return sFileName.substr(0, nLastDotPos);
    }
    else {
        return sFileName;
    }
}

Logger::Logger()
{
    sLogFolderPath = GetExeFilePath() + "\\" + "Log\\";
}

void Logger::Print(string sFunctionName, string format, ...)
{
	string sPrefix = "";
	ofstream fout((sLogFolderPath + sLatestFileName), ios::app);
	sPrefix += "[" + GetTime() + "]";
	sPrefix += " " + sFunctionName + "()";
	for (int i = 0; i < 30 - sFunctionName.size(); i++)
		sPrefix.append(" ");
	sPrefix += " : ";

	char sLog[1024] = "";
	va_list arg;
	va_start(arg, format.c_str());
	vsprintf_s(sLog, format.c_str(), arg);
	va_end(arg);

	string sPrintLog = sPrefix + sLog;

	if (this->GetFileSize() + sPrintLog.size() > DEFINE_10KB)
	{
		fout.close();
		CreateNewLog();
		fout.open(sLogFolderPath + sLatestFileName, ios::app);
	}

	cout << sPrintLog;
	fout << sPrintLog;
	fout.close();
	return;
}
string Logger::GetTime()
{
	struct timeb timer_msec;
	ftime(&timer_msec);
	int milli = timer_msec.millitm;

	time_t  timer;
	struct tm t;
	timer = time(NULL);
	localtime_s(&t, &timer);

	char result[100];
	sprintf_s(result, sizeof(result), "%02d.%02d.%02d_%02dh_%02dm_%02ds_%03dms", t.tm_year - 100, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, milli);
	return string(result);
}
int Logger::GetFileSize()
{
	ifstream fin(sLogFolderPath + sLatestFileName);
	streamoff size;

	fin.seekg(0, ios_base::end);
	size = fin.tellg();
	fin.close();
	return size;
}
void Logger::CreateNewLog()
{
	string sOldFileName = sLogFolderPath + sLatestFileName;
	string sNewFileName = "until_" + GetTime() + ".log";

	RenameFile(sOldFileName, sNewFileName);

	qLogFiles.push(sNewFileName);
	if (qLogFiles.size() >= 2)
	{
		TransFileToZip();
	}
}
