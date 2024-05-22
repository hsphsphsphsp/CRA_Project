#include <string>
#include "ssd.h"

class TestScript
{
public:
	TestScript(SSD* ssd) :
		ssd{ ssd }
	{}
	virtual bool DoScript() = 0;

protected:
	SSD* ssd;
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
	unsigned int nSSDSize = 100;
};