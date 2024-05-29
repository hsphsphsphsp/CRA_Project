#pragma once
#include "ssd.h"
#include "RunnerFileHandler.h"
#include "testscript.h"

class Runner
{
public:
	Runner(SSD* pSsd);

	bool CheckRunListFileExist(std::string& sCmd);

	void DoRunnerTestScenario();
private:
	Runner() { }
	Runner& operator=(const Runner& other) = delete;
	Runner(const Runner& other) = delete;

	TestScript* PrepareForEachTestScenario(std::string& EachCommand);

	void PrintTestScriptResult(bool isPassed);
	bool StartEachScenarioTest(TestScript* pTestScript);

	SSD* pSsd;
	RunnerFileHandler runnerFileHandler;
};