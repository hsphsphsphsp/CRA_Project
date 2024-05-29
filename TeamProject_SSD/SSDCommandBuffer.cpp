#include "SSDCommandBuffer.h"

void SSDCommandBuffer::OptimizeWriteCommand(CMD_BUFFER_MAP& nCmdBuffer, const unsigned int nWriteLBA)
{
	RemovePrevWriteCmdWithSameLBA(nCmdBuffer, nWriteLBA);

	DoNarrowRangeOfErase(nCmdBuffer, nWriteLBA);
}

void SSDCommandBuffer::OptimizeEraseComand(CMD_BUFFER_MAP& nCmdBuffer, unsigned int &nLBA, unsigned int &nSize)
{
	if (!umPrevEraseCommand.empty()) {
		MergeEraseCommand(nCmdBuffer, nLBA, nSize);
	}

	umPrevEraseCommand[{E, nLBA}] = nSize;

	RemovePrevWriteCmdInLBARange(nCmdBuffer, nLBA, nSize);
}

void SSDCommandBuffer::MergeEraseCommand(CMD_BUFFER_MAP& nCmdBuffer, unsigned int& nLBA, unsigned int& nData)
{
	auto it = umPrevEraseCommand.begin();

	unsigned int nPrevStartLBA = it->first.second;
	unsigned int nPrevEndLBA = it->first.second + it->second - 1;
	unsigned int nCurStartLBA = nLBA;
	unsigned int nCurEndLBA = nLBA + nData - 1;
	unsigned int nSize = 0;

	umPrevEraseCommand.clear();
	nSize = GetMergedSize(nPrevEndLBA, nCurEndLBA, nPrevStartLBA, nCurStartLBA);

	if (nPrevStartLBA <= nCurStartLBA)
	{
		if (!IsMergeable(nPrevEndLBA, nCurStartLBA, nSize))
		{
			return;
		}

		nLBA = nPrevStartLBA;
	}
	else
	{
		if (!IsMergeable(nCurEndLBA, nPrevStartLBA, nSize))
		{
			return;
		}

		nLBA = nCurStartLBA;
	}

	nData = nSize;

	nCmdBuffer.erase({ E, nPrevStartLBA });
}

bool SSDCommandBuffer::IsMergeable(unsigned int nEndLowLBA, unsigned int nStartHighLBA, unsigned int nSize)
{
	if ((nEndLowLBA + 1 < nStartHighLBA) || (nSize > MAX_ERASE_SIZE))
	{
		return false;
	}

	return true;
}

unsigned int SSDCommandBuffer::GetMergedSize(unsigned int nPrevEndLBA, unsigned int nCurEndLBA, unsigned int nPrevStartLBA, unsigned int nCurStartLBA)
{
	unsigned int nSize = 0;
	unsigned int nMergedStartLBA = nPrevStartLBA <= nCurStartLBA ? nPrevStartLBA : nCurStartLBA;

	if (nPrevEndLBA <= nCurEndLBA) {
		nSize = nCurEndLBA - nMergedStartLBA + 1;
	}
	else
	{
		nSize = nPrevEndLBA - nMergedStartLBA + 1;
	}

	return nSize;
}

void SSDCommandBuffer::RemovePrevWriteCmdInLBARange(CMD_BUFFER_MAP& nCmdBuffer, unsigned int nLBA, unsigned int nSize)
{
	for (int i = nLBA; i < nLBA + nSize; i++)
	{
		nCmdBuffer.erase({ W, i });
	}
}

void SSDCommandBuffer::DoNarrowRangeOfErase(CMD_BUFFER_MAP& nCmdBuffer, const unsigned int nWriteLBA, bool bRecursive)
{
	if (!ShouldExecuteNarrowRangeErase(nCmdBuffer, nWriteLBA, bRecursive))
	{
		return;
	}

	for (auto it = nCmdBuffer.begin(); it != nCmdBuffer.end(); /* no increment here */) {
		int nCmdType = it->first.first;

		if (nCmdType == W)
		{
			it++;
			continue;
		}

		unsigned int nEraseLBA = it->first.second;
		unsigned int nEraseSize = it->second;

		if (nWriteLBA == nEraseLBA)
		{
			if (nEraseSize > 1)
			{
				nCmdBuffer[{E, nEraseLBA + 1}] = nEraseSize - 1;
			}
			it = nCmdBuffer.erase(it);

			DoNarrowRangeOfErase(nCmdBuffer, nWriteLBA + 1, true);
			return;
		}
		else if (nWriteLBA == (nEraseLBA + (nEraseSize - 1)))
		{
			nCmdBuffer[{E, nEraseLBA}] -= 1;
			DoNarrowRangeOfErase(nCmdBuffer, nWriteLBA - 1, true);
			return;
		}
		else
		{
			it++;
			continue;
		}
	}
}

bool SSDCommandBuffer::ShouldExecuteNarrowRangeErase(CMD_BUFFER_MAP& nCmdBuffer, const unsigned int nWriteLBA, bool bRecursive)
{
	if (bRecursive == false) {
		return true;
	}

	if (nCmdBuffer.size() == 0 || nWriteLBA < 0)
	{
		return false;
	}

	auto it = nCmdBuffer.find({ W, nWriteLBA });
	if (it == nCmdBuffer.end())
	{
		return false;
	}
}

void SSDCommandBuffer::RemovePrevWriteCmdWithSameLBA(CMD_BUFFER_MAP& nCmdBuffer, const unsigned int nWriteLBA)
{
	nCmdBuffer.erase({ W, nWriteLBA });
}
