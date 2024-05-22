#include<iostream>
#include "ssd.h"
using namespace std;

class TestScript
{
public:
	TestScript(Ssd &ssd) : ssd(&ssd)
	{

	}
	TestScript()
	{

	}
	void setSsd(Ssd& ssd)
	{
		this->ssd = &ssd;
	}
	virtual void doScript() = 0;
protected:
	Ssd* ssd;
};
class TestScriptApp2 : public TestScript
{
public:
	TestScriptApp2(Ssd& ssd)
	{
		this->ssd = &ssd;
	}
	void doScript() override
	{
		ssd->Read(2);
		return;
	}
};
class TestScriptFactory
{
public:
	TestScript* createScript(string s, Ssd& ssd)
	{
		if (s.compare("testscriptapp2") == 0)
		{
			return new TestScriptApp2(ssd);
		}
	}
};