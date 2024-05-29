#pragma once
#include "SSDFileHandler.h"

using namespace std;

class SSDCommandBuffer
{
public:
	void OptimizeWriteCommand(CMD_BUFFER_MAP& nCmdBuffer, const unsigned int nWriteLBA);
	void OptimizeEraseComand(CMD_BUFFER_MAP& nCmdBuffer, unsigned int nLBA, unsigned int nSize);

private:
	void MergeEraseCommand(CMD_BUFFER_MAP& nCmdBuffer, unsigned int& nLBA, unsigned int& nSize);
	void RemovePrevWriteCmdInLBARange(CMD_BUFFER_MAP& nCmdBuffer, unsigned int nLBA, unsigned int nSize);
	void DoNarrowRangeOfErase(CMD_BUFFER_MAP& nCmdBuffer, const unsigned int nWriteLBA, bool bRecursive = 0);
	bool ShouldExecuteNarrowRangeErase(CMD_BUFFER_MAP& nCmdBuffer, const unsigned int nWriteLBA, bool bRecursive);
	void RemovePrevWriteCmdWithSameLBA(CMD_BUFFER_MAP& nCmdBuffer, const unsigned int nWriteLBA);
	bool IsMergeable(unsigned int nPrevEndLBA, unsigned int nCurStartLBA, unsigned int nSize);
	unsigned int GetMergedSize(unsigned int nPrevEndLBA, unsigned int nCurEndLBA, unsigned int nPrevStartLBA, unsigned int nCurStartLBA);

	CMD_BUFFER_MAP umPrevEraseCommand;
};