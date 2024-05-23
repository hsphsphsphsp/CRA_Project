#pragma once
#include <unordered_map>
#include "SSDFileHandler.h"

using namespace std;

class SSD
{
public:
	virtual unsigned int Read(unsigned int nLBA);
	virtual void Write(unsigned int nLBA, unsigned int nValue);
	virtual int GetSSDSize();

private:
	void ValidateParameter(unsigned int nLBA);
	bool IsLBAWritten(const unsigned int& nLBA, unordered_map<unsigned int, unsigned int>& umDataSet);
	
	SSDFileHandler ssdFileHandler;
	const int SSD_MAX_LBA = 99;
	const int DEFAULT_READ_VALUE = 0x00000000;
};