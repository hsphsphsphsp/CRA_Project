#include "ssd.h"

unsigned int SSD::Read(unsigned int nLBA)
{
	ValidateParameter(nLBA);

	unsigned int nReadValue = DEFAULT_READ_VALUE;
	
	unordered_map<unsigned int, unsigned int> umDataSet;
	ssdFileHandler.LoadNANDFile(umDataSet);

	if (IsLBAWritten(nLBA, umDataSet))
	{
		nReadValue = umDataSet[nLBA];
	}

	ssdFileHandler.WriteHexReadValueToResultFile(nReadValue);

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

void SSD::ValidateParameter(unsigned int nLBA, unsigned int nSize)
{
	if (nLBA < 0 || nLBA + nSize > SSD_MAX_LBA)
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
	}

	if (nCmdType == W)
	{
		OptimizeWriteCommand(nCmdBuffer, nLBA);
	}

	nCmdBuffer[{ nCmdType, nLBA }] = nData;

	ssdFileHandler.WriteCommandBufferFile(nCmdBuffer);
}

void SSD::OptimizeWriteCommand(CMD_BUFFER_MAP& nCmdBuffer, unsigned int& nLBA)
{
	RemovePrevWriteCmdWithSameLBA(nCmdBuffer, nLBA);
}

void SSD::RemovePrevWriteCmdWithSameLBA(CMD_BUFFER_MAP& nCmdBuffer, unsigned int& nLBA)
{
	nCmdBuffer.erase({ W, nLBA });
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