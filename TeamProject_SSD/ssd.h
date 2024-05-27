#pragma once
#include "SSDFileHandler.h"
#define R 0
#define W 1
#define E 2
using namespace std;

struct MyKey {
	int first;
	int second;
};

namespace std {
	template <>
	struct hash<MyKey> {
		size_t operator()(const MyKey& key) const {
			return hash<int>()(key.first) ^ hash<int>()(key.second);
		}
	};
}

namespace std {
	template <>
	struct equal_to<MyKey> {
		bool operator()(const MyKey& lhs, const MyKey& rhs) const {
			return lhs.first == rhs.first && lhs.second == rhs.second;
		}
	};
}

class SSD
{
public:
	virtual unsigned int Read(unsigned int nLBA);
	virtual void Write(unsigned int nLBA, unsigned int nValue);
	virtual void Erase(unsigned int nLBA, unsigned int nSize);
	virtual int GetSSDSize();

private:
	void ValidateParameter(unsigned int nLBA);
	bool IsLBAWritten(const unsigned int& nLBA, unordered_map<unsigned int, unsigned int>& umDataSet);
	void AddCommandToBuffer(int nCmdType, int nLBA, unsigned int nData);

	SSDFileHandler ssdFileHandler;
	const int SSD_MAX_LBA = 99;
	const int DEFAULT_READ_VALUE = 0x00000000;
};