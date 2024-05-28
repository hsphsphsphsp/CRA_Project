#pragma once
#include <string>
#include "ssd.h"

class ITestScript {
public:
	virtual bool DoScript() = 0;
};

class TestScript : ITestScript
{
public:
	TestScript(SSD* ssd) :
		ssd{ ssd }
	{
	}
	virtual bool DoScript() = 0;
	int GetSSDSize() const { return ssd->GetSSDSize(); }

protected:
	const unsigned int nExpectedDataInTestScenario = 0x5A5A5A5A;
	const unsigned int nLoopCountInTestScenario = 10;
	const unsigned int nTargetAddrInTestScenario = 0x0;

	void FullWriteInTestScript(unsigned int nWriteData);
	bool FullReadVerifyInTestScript(unsigned int nExpectedWrittenData);

	SSD* ssd;
};

class TestScriptApp1 : public TestScript
{
public:
	TestScriptApp1(SSD* ssd) : TestScript{ ssd } {}

	bool DoScript() override;
};

class TestScriptApp2 : public TestScript
{
public:
	TestScriptApp2(SSD* ssd) : TestScript{ ssd }
	{
		pTestData = (unsigned int*)malloc(sizeof(unsigned int*) * (IO_RANGE));
	}

	bool DoScript() override;
	void FirstWrite();
	void OverWrite();
	bool Verify();
private:
	const unsigned int IO_RANGE = 6;
	unsigned int* pTestData;
};

class FullWriteReadCompare : public TestScript
{
public:
	FullWriteReadCompare(SSD* ssd) : TestScript{ ssd } {}

	bool DoScript() override;
};

class FullRead10AndCompare : public TestScript
{
public:
	FullRead10AndCompare(SSD* ssd) : TestScript{ ssd } {}

	bool DoScript() override;
};

class Write10AndCompare : public TestScript
{
public:
	Write10AndCompare(SSD* ssd) : TestScript{ ssd } {}

	bool DoScript() override;
};

class Loop_WriteAndReadCompare : public TestScript
{
public:
	Loop_WriteAndReadCompare(SSD* ssd) : TestScript{ ssd } {}

	bool DoScript() override;
};

class TestScriptFactory
{
public:
	static TestScript* createScript(string sScriptName, SSD& ssd);
};
