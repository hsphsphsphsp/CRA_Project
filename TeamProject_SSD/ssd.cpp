#include "ssd.h"
#include <io.h>
#include <string>

void SSDFileHandler::ReadFromNANDFile(unordered_map<unsigned int, unsigned int>& umDataSet)
{
	string sIndex, sValue;

	if (IsNANDFileExist())
	{
		ifstream fin(sNandFileName);
		while (!fin.eof())
		{
			fin >> sIndex >> sValue;
			umDataSet.insert({ stoi(sIndex), stoul(sValue, nullptr, 16) });
		}
	}
}

void SSDFileHandler::WriteToNANDFile(std::unordered_map<unsigned int, unsigned int>& umDataSet)
{
	ofstream fout(sNandFileName);

	for (const auto& pair : umDataSet) 
	{
		fout << dec << pair.first << " " << "0x" << uppercase << hex << setw(8) << setfill('0') << pair.second << endl;
	}
}

void SSDFileHandler::WriteHexReadValueToResultFile(unsigned int nValue)
{
	ofstream fResultFile(sResultFileName);
	fResultFile << "0x" << uppercase << hex << setw(8) << setfill('0') << nValue;
}

bool SSDFileHandler::IsNANDFileExist()
{
	return _access(&*sNandFileName.begin(), 0) == 0;
}

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
