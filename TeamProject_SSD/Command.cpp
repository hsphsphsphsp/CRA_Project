#include "Command.h"
#include "ShellTestApp.h"

WriteCommand::WriteCommand(int nLba, int nData):
	nLba{nLba}, nData{nData}
{
}

void WriteCommand::execute(ShellTestApp* pApp)
{
	pApp->Write(nLba, nData);
}

ReadCommand::ReadCommand(int nLba) :
	nLba{nLba}
{
}

void ReadCommand::execute(ShellTestApp* pApp)
{
	pApp->Read(nLba);
}

void ExitCommand::execute(ShellTestApp* pApp)
{
	pApp->Exit();
}

void HelpCommand::execute(ShellTestApp* pApp)
{
	pApp->Help();
}

FullWriteCommand::FullWriteCommand(int nData) :
	nData{nData}
{
}

void FullWriteCommand::execute(ShellTestApp* pApp)
{
	pApp->FullWrite(nData);
}

void FullReadCommand::execute(ShellTestApp* pApp)
{
	pApp->FullRead();
}

void WrongCommand::execute(ShellTestApp* pApp)
{
	std::cout << "Wrong command, if you need to help, type \"help\"" << std::endl;
}
