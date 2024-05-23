#include "ssd.h"

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

}