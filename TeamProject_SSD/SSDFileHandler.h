#pragma once
#include <io.h>
#include <string>
#include <fstream>
#include <iomanip>
#include <unordered_map>
#include <sstream>

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

class SSDFileHandler
{
public:
	void LoadNANDFile(unordered_map<unsigned int, unsigned int>& umDataSet);
	void WriteNANDFile(const unordered_map<unsigned int, unsigned int>& umDataSet);
	void WriteHexReadValueToResultFile(unsigned int nValue);

	void LoadCommandBufferFile(unordered_map<MyKey, unsigned int>& nCmdBuffer);
	void WriteCommandBufferFile(const unordered_map<MyKey, unsigned int>& nCmdBuffer);

private:
	bool IsNANDFileExist();
	string FormatHex(unsigned int nValue);
	string FormatDec(unsigned int nValue);

	const string sResultFileName = "result.txt";
	const string sNandFileName = "nand.txt";
	string sCommandBufferFileName = "buffer.txt";
};
