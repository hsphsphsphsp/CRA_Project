#pragma once
#include <unordered_map>
using namespace std;

class SSD
{
public:
	virtual unsigned int Read(unsigned int nAddr);
	bool IsLBAWritten(const unsigned int& nAddr);
	virtual void Write(unsigned int nAddr, unsigned int value);

private:
	unordered_map<unsigned int, unsigned int> umDataSet;

	const int INVALID_DATA = 0x00000000;
};