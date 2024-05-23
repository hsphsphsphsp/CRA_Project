#include "testscript.h"

bool TestScriptApp1::DoScript()
{
	unsigned int nWriteValue = 0x0;

	for (int i = 0; i < GetSSDSize(); i++)
	{
		ssd->Write(i, nWriteValue);
	}
	for (int i = 0; i < GetSSDSize(); i++)
	{
		if (ssd->Read(i) != nWriteValue)
		{
			return false;
		}
	}
	return true;
}

bool TestScriptApp2::DoScript()
{
	unsigned int value = 1;
	const int WRITE_AREA = 5;
	int data[WRITE_AREA + 1];

	//1
	for (int nLoop = 0; nLoop < 30; nLoop++)
	{
		for (unsigned int nLba = 0; nLba <= 5; nLba++)
		{
			ssd->Write(nLba, value);
			data[nLba] = value;
		}
	}
	//2 - overwrite
	value = 2;
	for (unsigned int nLba = 0; nLba <= 5; nLba++)
	{
		ssd->Write(nLba, value);
		data[nLba] = value;
	}
	//3 - verify (read and compare)
	for (unsigned int nLba = 0; nLba <= 5; nLba++)
	{
		if (data[nLba] != ssd->Read(nLba))
		{
			return false;
		}
	}
	return true;
}

TestScript* TestScriptFactory::createScript(string sScriptName, SSD& ssd)
{
	if (sScriptName.compare("testscriptapp2") == 0)
	{
		return new TestScriptApp2(&ssd);
	}
	else if (sScriptName.compare("testscriptapp1") == 0)
	{
		return new TestScriptApp1(&ssd);
	}
	return nullptr;
}
