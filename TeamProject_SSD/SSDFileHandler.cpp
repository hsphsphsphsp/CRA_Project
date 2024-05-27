#include "SSDFileHandler.h"

void SSDFileHandler::LoadNANDFile(unordered_map<unsigned int, unsigned int>& umDataSet)
{
	string sIndex, sValue;

	if (IsNANDFileExist())
	{
		ifstream fin(sNandFileName);
		while (!fin.eof())
		{
			fin >> sIndex >> sValue;
			umDataSet.insert({ stoi(sIndex), stoul(sValue, nullptr, 16) });
		}
	}
}

void SSDFileHandler::WriteNANDFile(const unordered_map<unsigned int, unsigned int>& umDataSet)
{
	ofstream fout(sNandFileName);

	for (const auto& pair : umDataSet)
	{
		fout << FormatDec(pair.first) << " " << FormatHex(pair.second) << endl;
	}
}

void SSDFileHandler::WriteHexReadValueToResultFile(unsigned int nValue)
{
	ofstream fResultFile(sResultFileName);
	fResultFile << "0x" << FormatHex(nValue);
}

void SSDFileHandler::LoadCommandBufferFile(unordered_map<MyKey, unsigned int>& nCmdBuffer)
{
	ifstream fin(sCommandBufferFileName);
	string sCmdType, sLBA, sValue;

	if (fin.is_open())
	{
		while (!fin.eof())
		{
			fin >> sCmdType >> sLBA >> sValue;

			int nCmdType = sCmdType == "W" ? W : E;
			int nLBA = stoi(sLBA);
			unsigned int nValue = stoul(sValue, nullptr, 16);

			nCmdBuffer[{ nCmdType, nLBA }] = nValue;
		}
	}
}

void SSDFileHandler::WriteCommandBufferFile(const unordered_map<MyKey, unsigned int>& nCmdBuffer)
{
	ofstream fout(sCommandBufferFileName);

	for (const auto& it : nCmdBuffer)
	{
		if (it.first.first == W)
		{
			fout << "W" << " " << FormatDec(it.first.second) << " " << FormatHex(it.second) << endl;
		}
		else if (it.first.first == E)
		{
			fout << "E" << " " << FormatDec(it.first.second) << " " << FormatDec(it.second) << endl;
		}
	}
}

string SSDFileHandler::FormatHex(unsigned int nValue) 
{
	std::ostringstream oss;
	oss << "0x" << uppercase << hex << setw(8) << setfill('0') << nValue;
	return oss.str();
}

string SSDFileHandler::FormatDec(unsigned int nValue) 
{
	std::ostringstream oss;
	oss << dec << nValue;
	return oss.str();
}

bool SSDFileHandler::IsNANDFileExist()
{
	return _access(&*sNandFileName.begin(), 0) == 0;
}

void SSDFileHandler::removeNANDFile()
{
	remove(&*sNandFileName.begin());
}
