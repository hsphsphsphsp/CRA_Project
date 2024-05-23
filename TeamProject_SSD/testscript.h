#pragma once
#include <string>
#include "ssd.h"

class TestScript
{
public:
	TestScript(SSD* ssd) :
		ssd{ ssd }
	{
	}
	virtual bool DoScript() = 0;
	unsigned int GetSSDSize() const
	{
		return nSSDSize;
	}

protected:
	SSD* ssd;
private:
	unsigned int nSSDSize = 100;
};

class TestScriptApp1 : public TestScript
{
public:
	TestScriptApp1(SSD* ssd) : TestScript{ ssd } {}

	bool DoScript() override;
	bool FullReadVerify(unsigned int nWriteValue);
	void FullWrite(unsigned int nWriteValue);
};

class TestScriptApp2 : public TestScript
{
public:
	TestScriptApp2(SSD* ssd) : TestScript{ ssd }
	{
		pTestData = (unsigned int*)malloc(sizeof(unsigned int*) * WRITE_AREA);

	}

	bool DoScript() override;
	void FirstWrite();
	void OverWrite();
	bool Verify();
private:
	const int WRITE_AREA = 5;
	unsigned int* pTestData;
};

class TestScriptFactory
{
public:
	static TestScript* createScript(string sScriptName, SSD& ssd);
};