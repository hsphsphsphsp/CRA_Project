#include "ssd.h"

#include <io.h>
#include <string>

unsigned int SSD::Read(unsigned int nAddr)
{
	ValidateParameter(nAddr);

	unsigned int nReadValue = INVALID_DATA;
	unordered_map<unsigned int, unsigned int> umDataSet;

	ReadFromNAND(umDataSet);

	if (IsLBAWritten(umDataSet, nAddr))
	{
		nReadValue = umDataSet[nAddr];
	}

	WriteHexValueToFile(nReadValue);

	return nReadValue;
}

void SSD::ValidateParameter(unsigned int nAddr)
{
	if (nAddr < 0 || nAddr > 99) {
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
			umDataSet.insert({ stoi(sIndex), stoi(sValue, nullptr, 16) });
		}
		fin.close();
	}
}

void SSD::WriteHexValueToFile(unsigned int nValue)
{
	ofstream fResultFile(sResultFileName);
	fResultFile << "0x" << uppercase << hex << setw(8) << setfill('0') << nValue;
}

bool SSD::IsLBAWritten(std::unordered_map<unsigned int, unsigned int>& umDataSet, const unsigned int& nAddr)
{
	return umDataSet.find(nAddr) != umDataSet.end();
}

void SSD::Write(unsigned int nAddr, unsigned int value)
{
	ValidateParameter(nAddr);

	unordered_map<unsigned int, unsigned int> umDataSet;

	ReadFromNAND(umDataSet);

	umDataSet.insert({ nAddr, value });

	WriteToNAND(umDataSet);
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
