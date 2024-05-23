#pragma once
#include <io.h>
#include <string>
#include <fstream>
#include <iomanip>
#include <unordered_map>

using namespace std;

class SSDFileHandler
{
public:
	void ReadFromNANDFile(unordered_map<unsigned int, unsigned int>& umDataSet);
	void WriteToNANDFile(unordered_map<unsigned int, unsigned int>& umDataSet);
	void WriteHexReadValueToResultFile(unsigned int nValue);

private:
	bool IsNANDFileExist();

	const string sResultFileName = "result.txt";
	const string sNandFileName = "nand.txt";
};
