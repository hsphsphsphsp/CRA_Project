#include "ssd.h"

SSD::SSD()
{
	ssdFileHandler.RemoveCommandBufferFile();
}

unsigned int SSD::Read(unsigned int nLBA)
{
	ValidateParameter(nLBA);

	unsigned int nReadValue = DEFAULT_READ_VALUE;

	bool bCmdBufferReadSuccessed = ReadFromCmdBuffer(nLBA, nReadValue);

	if (!bCmdBufferReadSuccessed)
	{
		ReadFromNAND(nLBA, nReadValue);
	}
	return nReadValue;
}

void SSD::Write(unsigned int nLBA, unsigned int nValue)
{
	ValidateParameter(nLBA);

	if (bUseCommandBuffer) {
		AddCommandToBuffer(W, nLBA, nValue);
		return;
	}

	unordered_map<unsigned int, unsigned int> umDataSet;
	ssdFileHandler.LoadNANDFile(umDataSet);

	if (IsLBAWritten(nLBA, umDataSet))
	{
		umDataSet[nLBA] = nValue;
	}
	else
	{
		umDataSet.insert({ nLBA, nValue });
	}

	ssdFileHandler.WriteNANDFile(umDataSet);
}

void SSD::Erase(unsigned int nLBA, unsigned int nSize)
{
	ValidateParameter(nLBA, nSize - 1);

	if (bUseCommandBuffer) {
		AddCommandToBuffer(E, nLBA, nSize);
		return;
	}

	unordered_map<unsigned int, unsigned int> umDataSet;
	ssdFileHandler.LoadNANDFile(umDataSet);

	for (unsigned int i = 0; i < nSize; i++)
	{
		umDataSet.erase(nLBA + i);
	}

	if (umDataSet.empty())
	{
		ssdFileHandler.RemoveNANDFile();
		return;
	}

	ssdFileHandler.WriteNANDFile(umDataSet);
}

int SSD::GetSSDSize()
{
	return SSD_MAX_LBA + 1; // LBA is 0 base.
}

void SSD::ReadFromNAND(const unsigned int nLBA, unsigned int& nReadValue)
{
	unordered_map<unsigned int, unsigned int> umDataSet;
	ssdFileHandler.LoadNANDFile(umDataSet);

	if (IsLBAWritten(nLBA, umDataSet))
	{
		nReadValue = umDataSet[nLBA];
	}

	ssdFileHandler.WriteHexReadValueToResultFile(nReadValue);
}

bool SSD::ReadFromCmdBuffer(const unsigned int nLBA, unsigned int& nReadValue)
{
	CMD_BUFFER_MAP umCmdBuffer;
	ssdFileHandler.LoadCommandBufferFile(umCmdBuffer);

	if (!umCmdBuffer.empty())
	{
		// Read from latest Write
		auto it = umCmdBuffer.find({ W, nLBA });
		if (it != umCmdBuffer.end())
		{
			nReadValue = umCmdBuffer[{W, nLBA}];
			return true;
		}

		// Read from latest Erase
		for (auto it = umCmdBuffer.begin(); it != umCmdBuffer.end(); it++)
		{
			if (it->first.first == E)
			{
				if (it->first.second <= nLBA && nLBA <= it->first.second + (it->second - 1))
				{
					nReadValue = DEFAULT_READ_VALUE;
					return true;
				}
			}
		}

	}
	return false;
}

void SSD::ValidateParameter(unsigned int nLBA, unsigned int nSize)
{
	if (nLBA < 0 || nLBA + nSize > SSD_MAX_LBA || nSize >= MAX_ERASE_SIZE)
	{
		throw exception("INVALID COMMAND");
	}
}

bool SSD::IsLBAWritten(const unsigned int& nLBA, unordered_map<unsigned int, unsigned int>& umDataSet)
{
	return umDataSet.find(nLBA) != umDataSet.end();
}

void SSD::AddCommandToBuffer(int nCmdType, unsigned int nLBA, unsigned int nData)
{
	CMD_BUFFER_MAP nCmdBuffer;

	ssdFileHandler.LoadCommandBufferFile(nCmdBuffer);

	if (nCmdBuffer.size() >= 10) {
		Flush();
		nCmdBuffer.clear();
	}

	if (nCmdType == W)
	{
		OptimizeWriteCommand(nCmdBuffer, nLBA);
	}
	else if (nCmdType == E)
	{
		if (!umPrevEraseCommand.empty()) {
			MergeEraseCommand(nCmdBuffer, nLBA, nData);
		}

		umPrevEraseCommand[{E, nLBA}] = nData;

		OptimizeEraseComand(nCmdBuffer, nLBA, nData);
	}

	nCmdBuffer[{ nCmdType, nLBA }] = nData;

	ssdFileHandler.WriteCommandBufferFile(nCmdBuffer);
}

void SSD::MergeEraseCommand(CMD_BUFFER_MAP& nCmdBuffer, unsigned int& nLBA, unsigned int& nData)
{
	auto it = umPrevEraseCommand.begin();

	unsigned int nPrevStartLBA = it->first.second;
	unsigned int nPrevEndLBA = it->first.second + it->second - 1;
	unsigned int nCurStartLBA = nLBA;
	unsigned int nCurEndLBA = nLBA + nData - 1;
	unsigned int nSize = 0;

	nSize = GetMergedSize(nPrevEndLBA, nCurEndLBA, nPrevStartLBA);

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

bool SSD::IsMergeable(unsigned int nEndLowLBA, unsigned int nStartHighLBA, unsigned int nSize)
{
	if ((nEndLowLBA + 1 < nStartHighLBA) || (nSize > MAX_ERASE_SIZE))
	{
		umPrevEraseCommand.clear();
		return false;
	}

	return true;
}

unsigned int SSD::GetMergedSize(unsigned int nPrevEndLBA, unsigned int nCurEndLBA, unsigned int nPrevStartLBA)
{
	unsigned int nSize = 0;

	if (nPrevEndLBA <= nCurEndLBA) {
		nSize = nCurEndLBA - nPrevStartLBA + 1;
	}
	else
	{
		nSize = nPrevEndLBA - nPrevStartLBA + 1;
	}

	return nSize;
}

void SSD::OptimizeEraseComand(CMD_BUFFER_MAP& nCmdBuffer, unsigned int nLBA, unsigned int nSize)
{
	RemovePrevWriteCmdInLBARange(nCmdBuffer, nLBA, nSize);
}

void SSD::RemovePrevWriteCmdInLBARange(CMD_BUFFER_MAP& nCmdBuffer, unsigned int nLBA, unsigned int nSize)
{
	for (int i = nLBA; i < nLBA + nSize; i++)
	{
		nCmdBuffer.erase({ W, i });
	}
}

void SSD::OptimizeWriteCommand(CMD_BUFFER_MAP& nCmdBuffer, const unsigned int nWriteLBA)
{
	RemovePrevWriteCmdWithSameLBA(nCmdBuffer, nWriteLBA);

	DoNarrowRangeOfErase(nCmdBuffer, nWriteLBA);
}

void SSD::DoNarrowRangeOfErase(CMD_BUFFER_MAP& nCmdBuffer, const unsigned int nWriteLBA, bool bRecursive)
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

bool SSD::ShouldExecuteNarrowRangeErase(CMD_BUFFER_MAP& nCmdBuffer, const unsigned int nWriteLBA, bool bRecursive)
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

void SSD::RemovePrevWriteCmdWithSameLBA(CMD_BUFFER_MAP& nCmdBuffer, const unsigned int nWriteLBA)
{
	nCmdBuffer.erase({ W, nWriteLBA });
}

void SSD::Flush()
{
	CMD_BUFFER_MAP nCmdBuffer;

	ssdFileHandler.LoadCommandBufferFile(nCmdBuffer);

	bUseCommandBuffer = false;
	for (const auto& it : nCmdBuffer) {
		if (it.first.first == W)
		{
			Write(it.first.second, it.second);
		}
		else if (it.first.first == E)
		{
			Erase(it.first.second, it.second);
		}
	}
	bUseCommandBuffer = true;

	ssdFileHandler.RemoveCommandBufferFile();
}