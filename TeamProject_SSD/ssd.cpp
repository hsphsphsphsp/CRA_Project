#include "ssd.h"

#include <io.h>
#include <string>

unsigned int SSD::Read(unsigned int nAddr)
{
	unsigned int nReadValue = INVALID_DATA;

	if (IsLBAWritten(nAddr))
	{
		nReadValue = umDataSet[nAddr];
	}

	WriteHexValueToFile(nReadValue);

	return nReadValue;
}

void SSD::WriteHexValueToFile(unsigned int nValue)
{
	ofstream fResultFile(sResultFileName);
	fResultFile << "0x" << hex << setw(8) << setfill('0') << nValue;
}

bool SSD::IsLBAWritten(const unsigned int& nAddr)
{
	return umDataSet.find(nAddr) != umDataSet.end();
}

void SSD::Write(unsigned int nAddr, unsigned int value)
{
	SSD ssd;
	ifstream fin;
	ofstream fout;
	string strIndex, strValue;
	bool isFirstWrite = true;

	if (_access("nand.txt", 0) == 0) {
		isFirstWrite = false;
	}

	if (!isFirstWrite) {
		fin.open("nand.txt");
		while (!fin.eof())
		{
			fin >> strIndex >> strValue;
			umDataSet.insert({ stoi(strIndex), stoi(strValue, nullptr, 16) });
		}
		fin.close();
	}

	umDataSet.insert({ nAddr, value });
	fout.open("nand.txt");

	for (const auto& pair : umDataSet) {
		fout << dec << pair.first << " " << "0x" << hex << pair.second << endl;
	}

	fout.close();
}