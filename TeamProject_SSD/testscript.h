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
private:
	unsigned int nSSDSize = 100;
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
	TestScriptApp2(SSD* ssd) : TestScript{ ssd } {}

	bool DoScript() override;
};

class TestScriptFactory
{
public:
	TestScript* createScript(string sScriptName, SSD& ssd);
};