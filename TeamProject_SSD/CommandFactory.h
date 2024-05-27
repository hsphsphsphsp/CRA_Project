#pragma once
#include <queue>
#include <string>
#include <iostream>
#include "Command.h"

class CommandFactory {
public:
	Command* create(std::queue<std::string> qCmdBuffer);
};
