#include <string>
#include "ssd.h"

class TestScript
{
public:
	TestScript(Ssd* ssd) :
		ssd{ ssd }
	{}
	virtual bool DoScript() = 0;

protected:
	Ssd* ssd;
};

class TestScriptApp1 : public TestScript
{
public:
	TestScriptApp1(Ssd* ssd) : TestScript{ ssd } {}

	bool DoScript() override
	{
		unsigned int nWriteValue = 0x0;

		for (int i = 0; i < ssd->nSize; i++)
		{
			ssd->Write(i, nWriteValue);
		}
		for (int i = 0; i < ssd->nSize; i++)
		{
			if (ssd->Read(i) != nWriteValue)
			{
				return false;
			}
		}
		return true;
	}
};