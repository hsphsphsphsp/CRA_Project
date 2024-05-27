#pragma once
class ShellTestApp;

class Command {
public:
	virtual void execute(ShellTestApp* pApp) = 0;
};

class WriteCommand : public Command {
public:
	WriteCommand(int nLba, int nData);
	void execute(ShellTestApp* pApp) override;
private:
	int nLba;
	int nData;
};

class ReadCommand : public Command {
public:
	ReadCommand(int nLba);
	void execute(ShellTestApp* pApp) override;
private:
	int nLba;
};

class ExitCommand : public Command {
public:
	void execute(ShellTestApp* pApp) override;
};

class HelpCommand : public Command {
public:
	void execute(ShellTestApp* pApp) override;
};

class FullWriteCommand : public Command {
public:
	FullWriteCommand(int nData);
	void execute(ShellTestApp* pApp) override;
private:
	int nData;
};

class FullReadCommand : public Command {
public:
	void execute(ShellTestApp* pApp) override;
};

class WrongCommand : public Command {
public:
	void execute(ShellTestApp* pApp) override;
};
