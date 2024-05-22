#pragma once

class Ssd
{
public:
	virtual unsigned int Read(unsigned int nAddr);
	virtual void Write(unsigned int nAddr, unsigned int value);
};