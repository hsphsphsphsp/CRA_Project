#pragma once
#include "SSDFileHandler.h"

using namespace std;

class SSD
{
public:
	virtual unsigned int Read(unsigned int nLBA);
	virtual void Write(unsigned int nLBA, unsigned int nValue);
	virtual void Erase(unsigned int nLBA, unsigned int nSize);
	virtual int GetSSDSize();

private:
	void ValidateParameter(unsigned int nLBA, unsigned int nSize = 0);
	bool IsLBAWritten(const unsigned int& nLBA, unordered_map<unsigned int, unsigned int>& umDataSet);

	void AddCommandToBuffer(int nCmdType, unsigned int nLBA, unsigned int nData);
	void OptimizeWriteCommand(CMD_BUFFER_MAP& nCmdBuffer, unsigned int& nLBA);
	void RemovePrevWriteCmdWithSameLBA(CMD_BUFFER_MAP& nCmdBuffer, unsigned int& nLBA);

	SSDFileHandler ssdFileHandler;
	const unsigned int SSD_MAX_LBA = 99;
	const unsigned int DEFAULT_READ_VALUE = 0x00000000;
};