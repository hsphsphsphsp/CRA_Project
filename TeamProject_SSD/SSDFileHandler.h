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

struct pair_hash {
	template <class T1, class T2>
	std::size_t operator () (const std::pair<T1, T2>& p) const {
		auto hash1 = std::hash<T1>{}(p.first);
		auto hash2 = std::hash<T2>{}(p.second);
		return hash1 ^ (hash2 << 1);
	}
};

class SSDFileHandler
{
public:
	void LoadNANDFile(unordered_map<unsigned int, unsigned int>& umDataSet);
	void WriteNANDFile(const unordered_map<unsigned int, unsigned int>& umDataSet);
	void WriteHexReadValueToResultFile(unsigned int nValue);

	void LoadCommandBufferFile(unordered_map<pair<int, unsigned int>, unsigned int, pair_hash>& nCmdBuffer);
	void WriteCommandBufferFile(const unordered_map<pair<int, unsigned int>, unsigned int, pair_hash>& nCmdBuffer);

private:
	bool IsNANDFileExist();
	string FormatHex(unsigned int nValue);
	string FormatDec(unsigned int nValue);

	const string sResultFileName = "result.txt";
	const string sNandFileName = "nand.txt";
	string sCommandBufferFileName = "buffer.txt";
};
