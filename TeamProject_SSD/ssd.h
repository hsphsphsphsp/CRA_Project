#pragma once
#include "SSDFileHandler.h"

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
	void OptimizeEraseComand(CMD_BUFFER_MAP& nCmdBuffer, unsigned int nLBA, unsigned int nSize);
	void RemovePrevWriteCmdInLBARange(CMD_BUFFER_MAP& nCmdBuffer, unsigned int nLBA, unsigned int nSize);
	void OptimizeWriteCommand(CMD_BUFFER_MAP& nCmdBuffer, const unsigned int nWriteLBA);
	void DoNarrowRangeOfErase(CMD_BUFFER_MAP& nCmdBuffer, const unsigned int nWriteLBA, bool bRecursive = 0);
	bool ShouldExecuteNarrowRangeErase(CMD_BUFFER_MAP& nCmdBuffer, const unsigned int nWriteLBA, bool bRecursive);
	void RemovePrevWriteCmdWithSameLBA(CMD_BUFFER_MAP& nCmdBuffer, const unsigned int nWriteLBA);
	void MergeEraseCommand(CMD_BUFFER_MAP& nCmdBuffer, unsigned int &nLBA, unsigned int &nSize);

	CMD_BUFFER_MAP umPrevEraseCommand;
	SSDFileHandler ssdFileHandler;
	const unsigned int SSD_MAX_LBA = 99;
	const unsigned int MAX_ERASE_SIZE = 10;
	const unsigned int DEFAULT_READ_VALUE = 0x00000000;
	bool bUseCommandBuffer = true;
};