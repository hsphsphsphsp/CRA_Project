#pragma once
#include <unordered_map>
#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

class SSD
{
public:
	virtual unsigned int Read(unsigned int nAddr);
	virtual void Write(unsigned int nAddr, unsigned int value);
	virtual int GetSSDSize();

private:
	void ValidateParameter(unsigned int nAddr);
	void ReadFromNAND(std::unordered_map<unsigned int, unsigned int>& umDataSet);
	void WriteHexValueToFile(unsigned int nReadValue);
	bool IsLBAWritten(std::unordered_map<unsigned int, unsigned int>& umDataSet, const unsigned int& nAddr);
	void WriteToNAND(std::unordered_map<unsigned int, unsigned int>& umDataSet);
	
	const int MAX_LBA_COUNT = 100;
	const int INVALID_DATA = 0x00000000;
	const string sResultFileName = "result.txt";
	const string sNandFileName = "nand.txt";
};