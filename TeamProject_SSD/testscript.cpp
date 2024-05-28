#include "testscript.h"

void TestScript::FullWriteInTestScript(unsigned int nWriteData)
{
	for (int addrOffset = 0; addrOffset < ssd->GetSSDSize(); addrOffset++)
	{
		ssd->Write(addrOffset, nWriteData);
	}
}

bool TestScript::FullReadVerifyInTestScript(unsigned int nExpectedWrittenData)
{
	for (int addrOffset = 0; addrOffset < ssd->GetSSDSize(); addrOffset++)
	{
		if (nExpectedWrittenData != ssd->Read(addrOffset))
		{
			return false;
		}
	}
	return true;
}

bool TestScriptApp1::DoScript()
{
	unsigned int nWriteValue = 0x0;

	FullWriteInTestScript(nWriteValue);
	return FullReadVerifyInTestScript(nWriteValue);
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
	unsigned int nWriteValue = 0xAAAABBBB;
	for (int nLoop = 0; nLoop < 30; nLoop++)
	{
		for (unsigned int nLba = 0; nLba < IO_RANGE; nLba++)
		{
			ssd->Write(nLba, nWriteValue);
			pTestData[nLba] = nWriteValue;
		}
	}
}
void TestScriptApp2::OverWrite()
{
	unsigned int nWriteValue = 0x12345678;
	for (unsigned int nLba = 0; nLba < IO_RANGE; nLba++)
	{
		ssd->Write(nLba, nWriteValue);
		pTestData[nLba] = nWriteValue;
	}
}
bool TestScriptApp2::Verify()
{
	for (unsigned int nLba = 0; nLba < IO_RANGE; nLba++)
	{
		if (pTestData[nLba] != ssd->Read(nLba))
		{
			return false;
		}
	}
	return true;
}

bool FullWriteReadCompare::DoScript()
{
	FullWriteInTestScript(nExpectedDataInTestScenario);
	return FullReadVerifyInTestScript(nExpectedDataInTestScenario);
}

bool FullRead10AndCompare::DoScript()
{
	FullWriteInTestScript(nExpectedDataInTestScenario);

	bool isPass = true;
	for (int fullReadCount = 0; nLoopCountInTestScenario; fullReadCount++)
		isPass &= FullReadVerifyInTestScript(nExpectedDataInTestScenario);

	return isPass;
}

bool Write10AndCompare::DoScript()
{
	for (int testCount = 0; testCount < nLoopCountInTestScenario; testCount++)
		ssd->Write(nTargetAddrInTestScenario, nExpectedDataInTestScenario);

	if (nExpectedDataInTestScenario != ssd->Read(nTargetAddrInTestScenario))
		return false;

	return true;
}

bool Loop_WriteAndReadCompare::DoScript()
{
	for (int testCount = 0; testCount < nLoopCountInTestScenario; testCount++)
	{
		ssd->Write(nTargetAddrInTestScenario, nExpectedDataInTestScenario);
		if (nExpectedDataInTestScenario != ssd->Read(nTargetAddrInTestScenario))
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
	else if (sScriptName.compare("FullWriteReadCompare") == 0)
	{
		return new FullWriteReadCompare(&ssd);
	}
	else if (sScriptName.compare("FullRead10AndCompare") == 0)
	{
		return new FullRead10AndCompare(&ssd);
	}
	else if (sScriptName.compare("Write10AndCompare") == 0)
	{
		return new Write10AndCompare(&ssd);
	}
	else if (sScriptName.compare("Loop_WriteAndReadCompare") == 0)
	{
		return new Loop_WriteAndReadCompare(&ssd);
	}
	return nullptr;
}
