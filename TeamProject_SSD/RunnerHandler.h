#pragma once
#include "RunnerFileHandler.h"
#include <iostream>
#include <Vector>
#include <stdexcept>
#include "ShellTestApp.h"
using namespace std;

class RunnerHandler
{
public:
	static RunnerHandler& getInstance() {
		static SSD ssd;
		static ShellTestApp shellTestApp(&ssd);
		static RunnerHandler instance{ &shellTestApp };
		return instance;
	}

	bool Runner();
private:
	
	RunnerHandler(ShellTestApp* shellTestApp) : shellTestApp(shellTestApp) {}
	RunnerHandler& operator=(const RunnerHandler& other) = delete;
	RunnerHandler(const RunnerHandler& other) = delete;

	bool DoTestScenario();

	ShellTestApp* shellTestApp;
	RunnerFileHandler runnerFileHandler;
	vector<string> commandList;

};