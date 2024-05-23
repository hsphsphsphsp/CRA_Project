#include "ssd.h"

unsigned int SSD::Read(unsigned int nAddr)
{
	ofstream outFile("result.txt");
	unsigned int nData = INVALID_DATA;

	if (IsLBAWritten(nAddr))
	{
		nData = umDataSet[nAddr];
	}

	outFile << "0x" << hex << setw(8) << setfill('0') << nData;

	return nData;
}

bool SSD::IsLBAWritten(const unsigned int& nAddr)
{
	return umDataSet.find(nAddr) != umDataSet.end();
}

void SSD::Write(unsigned int nAddr, unsigned int value)
{

}