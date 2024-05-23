#pragma once
#include <unordered_map>
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

class SSD
{
public:
	virtual unsigned int Read(unsigned int nLBA);
	virtual void Write(unsigned int nLBA, unsigned int nValue);
	virtual int GetSSDSize();

private:
	void ValidateParameter(unsigned int nLBA);
	void ReadFromNAND(std::unordered_map<unsigned int, unsigned int>& umDataSet);
	bool IsLBAWritten(const unsigned int& nLBA, std::unordered_map<unsigned int, unsigned int>& umDataSet);
	void WriteHexReadValueToResultFile(unsigned int nReadValue);
	void WriteToNAND(std::unordered_map<unsigned int, unsigned int>& umDataSet);
	
	const int MAX_LBA_COUNT = 100;
	const int DEFAULT_READ_VALUE = 0x00000000;
	const string sResultFileName = "result.txt";
	const string sNandFileName = "nand.txt";
};
