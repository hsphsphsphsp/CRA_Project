#include "logger.h"

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

void Logger::Print(string sLog, string sFunctionName)
{
	string result;
	ofstream fout((sLogFolderPath + sLatestFileName), ios::app);
	result += GetTime();
	result += " " + sFunctionName + "()";
	for (int i = 0; i < 30 - sFunctionName.size(); i++)
		result.append(" ");
	result += " : " + sLog;

	if (this->GetFileSize() + result.size() > DEFINE_10KB)
	{
		fout.close();
		CreateNewLog();
		fout.open(sLogFolderPath + sLatestFileName, ios::app);
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
	sprintf_s(result, sizeof(result), "[%02d.%02d.%02d %02d:%02d]", t.tm_year - 100, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min);
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
	time_t  timer;
	struct tm t;
	timer = time(NULL);
	localtime_s(&t, &timer);

	string sOldFileName;
	string sNewFileName;
	char sTimeData[100];
	sOldFileName = sLogFolderPath + sLatestFileName;
	sprintf_s(sTimeData, sizeof(sTimeData), "%02d%02d%02d_%02dh_%02dm_%02ds.log", t.tm_year - 100, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
	sNewFileName += "until_" + string(sTimeData);

	string command = "ren " + sOldFileName + " " + sNewFileName;

	int result = std::system(command.c_str());

	if (result == 0) {
		std::cout << "Success to rename log file." << std::endl;
		qLogFiles.push(sNewFileName);
		if (qLogFiles.size() >= 2)
		{
			TransFileToZip();
		}
	}
	else {
		std::cerr << "Fail to rename log file." << std::endl;
	}
}
