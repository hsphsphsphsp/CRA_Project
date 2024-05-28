#pragma once
#include<iostream>
#include "ssd.h"
#include "testscript.h"

class Command {
public:
	Command(SSD* pSsd, std::string sCmdName);
	virtual void execute() = 0;
	std::string sCmdName = "DefaultCommand";
protected:
	SSD* pSsd = nullptr;
	void PrintBlockData(unsigned int nLba, unsigned int nData);
};

class WriteCommand : public Command {
public:
	WriteCommand(SSD* pSsd, int nLba, int nData);
	void execute() override;
private:
	int nLba;
	int nData;
};

class ReadCommand : public Command {
public:
	ReadCommand(SSD* pSsd, int nLba);
	void execute() override;
private:
	int nLba;
};

class ExitCommand : public Command {
public:
	ExitCommand(SSD* pSsd);
	void execute() override;
};

class HelpCommand : public Command {
public:
	HelpCommand(SSD* pSsd);
	void execute() override;
};

class FullWriteCommand : public Command {
public:
	FullWriteCommand(SSD* pSsd, int nData);
	void execute() override;
private:
	int nData;
};

class FullReadCommand : public Command {
public:
	FullReadCommand(SSD* pSsd);
	void execute() override;
};

class DoScriptCommand : public Command {
public:
	DoScriptCommand(SSD* pSsd, std::string sTestScriptName);
	void execute() override;
private:
	std::string sTestScriptName;
};

class DoRunnerCommand : public Command {
public:
	DoRunnerCommand(SSD* pSsd, std::string sTestScriptName);
	void execute() override;
private:
	std::string sTestScriptName;
};

class EraseCommand : public Command {
public:
	EraseCommand(SSD* pSsd, int nLba, int nSize);
	void execute() override;
private:
	int nLba;
	int nSize;
};

class RunListCommmand : public Command {
public:
	RunListCommmand(SSD* pSsd, std::string sFilename);
	void execute() override;
private:
	std::string sFilename;
};

class WrongCommand : public Command {
public:
	WrongCommand(SSD* pSsd);
	void execute() override;
};
