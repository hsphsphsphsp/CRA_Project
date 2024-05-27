#pragma once
#include "ssd.h"
#include "RunnerFileHandler.h"
#include "testscript.h"

class RunnerHandler
{
public:
	RunnerHandler(SSD* pSsd);

	bool CheckRunListFileExist(std::string& sCmd);

	void DoRunnerTestScenario();
private:
	RunnerHandler() { }
	RunnerHandler& operator=(const RunnerHandler& other) = delete;
	RunnerHandler(const RunnerHandler& other) = delete;

	TestScript* PrepareForEachTestScenario(std::string& EachCommand);

	void PrintRunnerResult(bool isPassed);
	bool StartEachScenarioTest(TestScript* pTestScript);

	SSD* pSsd;
	RunnerFileHandler runnerFileHandler;
};