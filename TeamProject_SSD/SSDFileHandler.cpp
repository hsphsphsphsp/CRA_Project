#include "SSDFileHandler.h"

void SSDFileHandler::ReadFromNANDFile(unordered_map<unsigned int, unsigned int>& umDataSet)
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

void SSDFileHandler::WriteToNANDFile(std::unordered_map<unsigned int, unsigned int>& umDataSet)
{
	ofstream fout(sNandFileName);

	for (const auto& pair : umDataSet)
	{
		fout << dec << pair.first << " " << "0x" << uppercase << hex << setw(8) << setfill('0') << pair.second << endl;
	}
}

void SSDFileHandler::WriteHexReadValueToResultFile(unsigned int nValue)
{
	ofstream fResultFile(sResultFileName);
	fResultFile << "0x" << uppercase << hex << setw(8) << setfill('0') << nValue;
}

bool SSDFileHandler::IsNANDFileExist()
{
	return _access(&*sNandFileName.begin(), 0) == 0;
}