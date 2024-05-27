#include "ssd.h"
#include <vector>

unsigned int SSD::Read(unsigned int nLBA)
{
	ValidateParameter(nLBA);

	unsigned int nReadValue = DEFAULT_READ_VALUE;
	
	unordered_map<unsigned int, unsigned int> umDataSet;
	ssdFileHandler.ReadFromNANDFile(umDataSet);

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

	AddCommandToBuffer(W, nLBA, nValue);

	unordered_map<unsigned int, unsigned int> umDataSet;
	ssdFileHandler.ReadFromNANDFile(umDataSet);

	if (IsLBAWritten(nLBA, umDataSet))
	{
		umDataSet[nLBA] = nValue;
	}
	else
	{
		umDataSet.insert({ nLBA, nValue });
	}

	ssdFileHandler.WriteToNANDFile(umDataSet);
}

void SSD::Erase(unsigned int nLBA, unsigned int nSize)
{
	if (nLBA < 0 || nLBA + nSize - 1 > SSD_MAX_LBA)
	{
		throw exception("INVALID COMMAND");
	}

	AddCommandToBuffer(E, nLBA, nSize);

	unordered_map<unsigned int, unsigned int> umDataSet;
	ssdFileHandler.ReadFromNANDFile(umDataSet);

	for (int i = 0; i < nSize; i++)
	{
		umDataSet.erase(nLBA + i);
	}

	if (umDataSet.empty())
	{
		remove("nand.txt");
		return;
	}

	ssdFileHandler.WriteToNANDFile(umDataSet);
}

int SSD::GetSSDSize()
{
	return SSD_MAX_LBA + 1; // LBA is 0 base.
}

void SSD::ValidateParameter(unsigned int nLBA)
{
	if (nLBA < 0 || nLBA > SSD_MAX_LBA)
	{
		throw exception("INVALID COMMAND");
	}
}

bool SSD::IsLBAWritten(const unsigned int& nLBA, unordered_map<unsigned int, unsigned int>& umDataSet)
{
	return umDataSet.find(nLBA) != umDataSet.end();
}

void SSD::AddCommandToBuffer(int nCmdType, int nLBA, unsigned int nData)
{
	unordered_map<MyKey, unsigned int> nCmdBuffer;

	string sCommandBufferFileName = "buffer.txt";
	ifstream fin(sCommandBufferFileName);

	if (fin.is_open())
	{
		while (!fin.eof())
		{
			string sCmdType, sLBA, sValue;
			fin >> sCmdType >> sLBA >> sValue;
			if (sCmdType == "") break;

			int nCmdType = sCmdType == "W" ? W : E;
			int nLBA = stoi(sLBA);
			unsigned int nValue = stoul(sValue, nullptr, 16);

			nCmdBuffer[{ nCmdType, nLBA }] = nValue;
		}
	}

	nCmdBuffer[{ nCmdType, nLBA }] = nData;

	ofstream fout(sCommandBufferFileName);

	for (const auto& it : nCmdBuffer)
	{
		if (it.first.first == W)
		{
			fout << "W" << " " << dec << it.first.second << " " << "0x" << uppercase << hex << setw(8) << setfill('0') << it.second << endl;
		}
		else if (it.first.first == E)
		{
			fout << "E" << " " << dec << it.first.second << " " << it.second << endl;
		}
	}
}