#include "ssd.h"

unsigned int SSD::Read(unsigned int nAddr)
{
	if (IsLBAWritten(nAddr) == false)
	{
		return INVALID_DATA;
	}
	
	return umDataSet[nAddr];
}

bool SSD::IsLBAWritten(const unsigned int& nAddr)
{
	return umDataSet.find(nAddr) == umDataSet.end();
}

void SSD::Write(unsigned int nAddr, unsigned int value)
{

}