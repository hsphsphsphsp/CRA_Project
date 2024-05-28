#pragma once
#include <io.h>
#include <string>
#include <fstream>
#include <iomanip>
#include <unordered_map>
#include <sstream>
#include "logger.h"

#include "Type.h"

using namespace std;

class SSDFileHandler
{
public:
	void LoadNANDFile(unordered_map<unsigned int, unsigned int>& umDataSet);
	void WriteNANDFile(const unordered_map<unsigned int, unsigned int>& umDataSet);
	void WriteHexReadValueToResultFile(unsigned int nValue);
	void RemoveNANDFile();
	void RemoveCommandBufferFile();

	void LoadCommandBufferFile(CMD_BUFFER_MAP& nCmdBuffer);
	void WriteCommandBufferFile(const CMD_BUFFER_MAP& nCmdBuffer);

private:
	bool IsNANDFileExist();
	int GetCmdType(string& sCmdType);

	string FormatHex(unsigned int nValue);
	string FormatDec(unsigned int nValue);
	unsigned int StringToUInt(const string& sValue, int numType);

	const string sResultFileName = "result.txt";
	const string sNandFileName = "nand.txt";
	string sCommandBufferFileName = "buffer.txt";
};
