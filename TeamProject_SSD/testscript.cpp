#include <string>
#include "ssd.h"

class TestScript
{
public:
	TestScript(SSD* ssd) :
		ssd{ ssd }
	{}
	virtual bool DoScript() = 0;
	unsigned int GetSSDSize() const
	{
		return nSSDSize;
	}

protected:
	SSD* ssd;
	unsigned int nSSDSize = 100;
};

class TestScriptApp1 : public TestScript
{
public:
	TestScriptApp1(SSD* ssd) : TestScript{ ssd } {}

	bool DoScript() override
	{
		unsigned int nWriteValue = 0x0;
		
		for (int i = 0; i < nSSDSize; i++)
		{
			ssd->Write(i, nWriteValue);
		}
		for (int i = 0; i < nSSDSize; i++)
		{
			if (ssd->Read(i) != nWriteValue)
			{
				return false;
			}
		}
		return true;
	}
};
class TestScriptApp2 : public TestScript
{
public:
	TestScriptApp2(SSD* ssd) : TestScript{ ssd } {}
	bool DoScript() override
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

};
class TestScriptFactory
{
public:
	TestScript* createScript(string sScriptName, SSD& ssd)
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
};