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
