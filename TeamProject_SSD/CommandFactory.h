#pragma once
#include <queue>
#include <string>
#include <iostream>
#include"Command.h"

class CommandSingletonFactory {
public:
	CommandSingletonFactory() = default;
	static CommandSingletonFactory& GetInstance() {
		static CommandSingletonFactory cfInstance;
		return cfInstance;
	}
	CommandSingletonFactory(const CommandSingletonFactory&) = delete;
	CommandSingletonFactory& operator=(const CommandSingletonFactory&) = delete;
	CommandSingletonFactory(CommandSingletonFactory&&) = delete;
	CommandSingletonFactory& operator=(CommandSingletonFactory&&) = delete;

	Command* create(SSD* pSsd, std::queue<std::string> qCmdBuffer);
private:
	std::string sCmd;
	unsigned int nLba;
	unsigned int nData;
	unsigned int nSize;
	unsigned int nStartLba;
	unsigned int nEndLba;

	void AssertArguments(SSD* pSsd, std::queue<std::string> qCmdBuffer);
	unsigned int ConvertToNumFrom(std::string sNumber);
};
