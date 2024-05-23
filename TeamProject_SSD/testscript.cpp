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
	FirstWrite();
	OverWrite();
	if (Verify() != true)
	{
		return false;
	}
	return true;
}
void TestScriptApp2::FirstWrite()
{
	unsigned int value = 0xAAAABBBB;
	for (int nLoop = 0; nLoop < 30; nLoop++)
	{
		for (unsigned int nLba = 0; nLba <= WRITE_AREA; nLba++)
		{
			ssd->Write(nLba, value);
			pTestData[nLba] = value;
		}
	}
}
void TestScriptApp2::OverWrite()
{
	unsigned int value = 0x12345678;
	for (unsigned int nLba = 0; nLba <= WRITE_AREA; nLba++)
	{
		ssd->Write(nLba, value);
		pTestData[nLba] = value;
	}
}
bool TestScriptApp2::Verify()
{
	for (unsigned int nLba = 0; nLba <= WRITE_AREA; nLba++)
	{
		if (pTestData[nLba] != ssd->Read(nLba))
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
