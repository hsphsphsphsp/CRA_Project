#pragma once
#include "SSDFileHandler.h"
#include "SSDCommandBuffer.h"

using namespace std;

class SSD
{
public:
	SSD();
	virtual unsigned int Read(unsigned int nLBA);
	virtual void Write(unsigned int nLBA, unsigned int nValue);
	virtual void Erase(unsigned int nLBA, unsigned int nSize);
	virtual void Flush();
	virtual int GetSSDSize();

private:
	void ReadFromNAND(const unsigned int nLBA, unsigned int& nReadValue);
	bool ReadFromCmdBuffer(const unsigned int nLBA, unsigned int& nReadValue);

	void ValidateParameter(unsigned int nLBA, unsigned int nSize = 0);
	bool IsLBAWritten(const unsigned int& nLBA, unordered_map<unsigned int, unsigned int>& umDataSet);

	void AddCommandToBuffer(int nCmdType, unsigned int nLBA, unsigned int nData);
	void OptimizeCommandBuffer(int nCmdType, CMD_BUFFER_MAP& nCmdBuffer, unsigned int nLBA, unsigned int nData);
	void DoFlush(CMD_BUFFER_MAP& nCmdBuffer);
	bool IsCommandBufferFull(CMD_BUFFER_MAP& nCmdBuffer);

	SSDFileHandler ssdFileHandler;
	SSDCommandBuffer ssdCmdBuffer;

	const unsigned int SSD_MAX_LBA = 99;
	const unsigned int DEFAULT_READ_VALUE = 0x00000000;
	bool bUseCommandBuffer = true;
};