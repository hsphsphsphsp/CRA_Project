#include "ssd.h"
#include <io.h>
#include <string>

unsigned int SSD::Read(unsigned int nLBA)
{
	ValidateParameter(nLBA);

	unsigned int nReadValue = DEFAULT_READ_VALUE;
	unordered_map<unsigned int, unsigned int> umDataSet;

	ReadFromNAND(umDataSet);

	if (IsLBAWritten(nLBA, umDataSet))
	{
		nReadValue = umDataSet[nLBA];
	}

	WriteHexReadValueToResultFile(nReadValue);

	return nReadValue;
}

void SSD::Write(unsigned int nLBA, unsigned int nValue)
{
	ValidateParameter(nLBA);

	unordered_map<unsigned int, unsigned int> umDataSet;

	ReadFromNAND(umDataSet);

	if (IsLBAWritten(nLBA, umDataSet))
	{
		umDataSet[nLBA] = nValue;
	}
	else
	{
		umDataSet.insert({ nLBA, nValue });
	}

	WriteToNAND(umDataSet);
}

int SSD::GetSSDSize()
{
	return MAX_LBA_COUNT;
}

void SSD::ValidateParameter(unsigned int nLBA)
{
	if (nLBA < 0 || nLBA > 99) {
		throw exception("INVALID COMMAND");
	}
}

void SSD::ReadFromNAND(std::unordered_map<unsigned int, unsigned int>& umDataSet)
{
	ifstream fin;
	string sIndex, sValue;

	if (_access(&*sNandFileName.begin(), 0) == 0)
	{
		fin.open(sNandFileName);
		while (!fin.eof())
		{
			fin >> sIndex >> sValue;
			umDataSet.insert({ stoi(sIndex), stoul(sValue, nullptr, 16) });
		}
		fin.close();
	}
}

bool SSD::IsLBAWritten(const unsigned int& nLBA, std::unordered_map<unsigned int, unsigned int>& umDataSet)
{
	return umDataSet.find(nLBA) != umDataSet.end();
}

void SSD::WriteHexReadValueToResultFile(unsigned int nValue)
{
	ofstream fResultFile(sResultFileName);
	fResultFile << "0x" << uppercase << hex << setw(8) << setfill('0') << nValue;
}

void SSD::WriteToNAND(std::unordered_map<unsigned int, unsigned int>& umDataSet)
{
	ofstream fout;

	fout.open(sNandFileName);
	for (const auto& pair : umDataSet) {
		fout << dec << pair.first << " " << "0x" << uppercase << hex << setw(8) << setfill('0') << pair.second << endl;
	}
	fout.close();
}
