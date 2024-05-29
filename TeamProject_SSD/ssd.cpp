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

void SSD::AddCommandToBuffer(int nCmdType, unsigned int nLBA, unsigned int nData)
{
	CMD_BUFFER_MAP nCmdBuffer;

	ssdFileHandler.LoadCommandBufferFile(nCmdBuffer);

	if (IsCommandBufferFull(nCmdBuffer)) {
		DoFlush(nCmdBuffer);
	}

	OptimizeCommandBuffer(nCmdType, nCmdBuffer, nLBA, nData);

	nCmdBuffer[{ nCmdType, nLBA }] = nData;

	ssdFileHandler.WriteCommandBufferFile(nCmdBuffer);
}

void SSD::OptimizeCommandBuffer(int nCmdType, CMD_BUFFER_MAP& nCmdBuffer, unsigned int nLBA, unsigned int nData)
{
	if (nCmdType == W)
	{
		ssdCmdBuffer.OptimizeWriteCommand(nCmdBuffer, nLBA);
	}
	else if (nCmdType == E)
	{
		ssdCmdBuffer.OptimizeEraseComand(nCmdBuffer, nLBA, nData);
	}
}

void SSD::DoFlush(CMD_BUFFER_MAP& nCmdBuffer)
{
	Flush();
	nCmdBuffer.clear();
}

bool SSD::IsCommandBufferFull(CMD_BUFFER_MAP& nCmdBuffer)
{
	return nCmdBuffer.size() >= MAX_ERASE_SIZE;
}
