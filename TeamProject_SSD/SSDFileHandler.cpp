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
			umDataSet.insert({ StringToUInt(sIndex, HEX), StringToUInt(sValue, HEX) });
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

void SSDFileHandler::RemoveNANDFile()
{
	remove(&*sNandFileName.begin());
}

void SSDFileHandler::RemoveCommandBufferFile()
{
	remove(&*sCommandBufferFileName.begin());
}

void SSDFileHandler::LoadCommandBufferFile(CMD_BUFFER_MAP& nCmdBuffer)
{
	ifstream fin(sCommandBufferFileName);
	string sCmdType, sLBA, sValue;

	if (fin.is_open())
	{
		while (!fin.eof())
		{
			fin >> sCmdType >> sLBA >> sValue;

			int nCmdType = GetCmdType(sCmdType);
			unsigned int nLBA = StringToUInt(sLBA, DEC);
			unsigned int nValue = StringToUInt(sValue, HEX);

			nCmdBuffer[{ nCmdType, nLBA }] = nValue;
		}
	}
}

void SSDFileHandler::WriteCommandBufferFile(const CMD_BUFFER_MAP& nCmdBuffer)
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

bool SSDFileHandler::IsNANDFileExist()
{
	return _access(&*sNandFileName.begin(), 0) == 0;
}

int SSDFileHandler::GetCmdType(std::string& sCmdType)
{
	return sCmdType == "W" ? W : E;
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

unsigned int SSDFileHandler::StringToUInt(const string& sValue, int numType)
{
	try 
	{
		if (numType == HEX)
			return stoul(sValue, nullptr, 16);

		return stoul(sValue);
	}
	catch (const std::invalid_argument& e) 
	{
		throw exception("Invalid argument");
	}
	catch (const std::out_of_range& e) 
	{
		throw exception("Out of range");
	}
}
