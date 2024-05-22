#include "ssd.h"

unsigned int SSD::Read(unsigned int nAddr)
{
	if (umDataSet.find(nAddr) == umDataSet.end()) 
	{
		return INVALID_DATA;
	}
	else
	{
		return umDataSet[nAddr];
	}
}
void SSD::Write(unsigned int nAddr, unsigned int value)
{

}