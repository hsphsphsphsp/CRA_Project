#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <fstream>
#include "../TeamProject_SSD/ssd.cpp"
#include "../TeamProject_SSD/testscript.cpp"
#include "../TeamProject_SSD/ShellTestApp.cpp"

using namespace std;
using namespace testing;


class MockSSD : public SSD {
public:
	MOCK_METHOD(unsigned int, Read, (unsigned int), (override));
	MOCK_METHOD(void, Write, (unsigned int, unsigned int), (override));
};

class TestScriptAppFixture : public Test
{
public:
	const string SCRIPT_APP1 = "testscriptapp1";
	const string SCRIPT_APP2 = "testscriptapp2";

	NiceMock<MockSSD> mockSSD;
	TestScriptFactory fTestScriptFactory;
	TestScript* pTestScript;
protected:
	void MakeScript(string scriptName)
	{
		pTestScript = fTestScriptFactory.createScript(scriptName, mockSSD);
	}
	
};

TEST_F(TestScriptAppFixture, TestScriptApp1_ConfirmCallFullWrite) {
	MakeScript(SCRIPT_APP1);

	EXPECT_CALL(mockSSD, Write)
		.Times(pTestScript->GetSSDSize());

	pTestScript->DoScript();
}

TEST_F(TestScriptAppFixture, TestScriptApp1_ConfirmCallFullRead) {
	MakeScript(SCRIPT_APP1);

	EXPECT_CALL(mockSSD, Read)
		.Times(pTestScript->GetSSDSize());

	pTestScript->DoScript();
}

TEST_F(TestScriptAppFixture, TestScriptApp1_FailReadVerify) {
	MakeScript(SCRIPT_APP1);

	EXPECT_CALL(mockSSD, Read)
		.WillOnce(Return(0x0))
		.WillOnce(Return(0x0))
		.WillRepeatedly(Return(0xFF));

	EXPECT_THAT(pTestScript->DoScript(), Eq(false));
}
TEST(TestScriptFactory, TestScriptFactoryNull)
{
	MockSSD mockSSD;
	TestScript* tScript;
	string sScriptName = "testscriptapp_invalid";
	TestScriptFactory fTestScriptFactory;

	tScript = fTestScriptFactory.createScript(sScriptName, mockSSD);
	EXPECT_THAT(tScript, Eq(nullptr));
}

TEST(TestScript, TestShellCallTestScript)
{
	MockSSD mockSSD;
	ShellTestApp shellTestApp(&mockSSD);
	
	EXPECT_CALL(mockSSD, Read)
		.Times(AtLeast(1));

	shellTestApp.DoScript("testscriptapp1");
}

TEST_F(TestScriptAppFixture, TestScriptApp2_CheckWriteAgingExecutedCount)
{
	MakeScript(SCRIPT_APP2);
	unsigned int nValue = 0xAAAABBBB;
	for (unsigned int nLba = 0; nLba <= 5; nLba++)
		EXPECT_CALL(mockSSD, Write(nLba, nValue)).Times(30);
	nValue = 0x12345678;
	for (unsigned int nLba = 0; nLba <= 5; nLba++)
		EXPECT_CALL(mockSSD, Write(nLba, nValue)).Times(1);
	pTestScript->DoScript();
}

TEST_F(TestScriptAppFixture, TestScriptApp2_CheckWhenVerifySuccess)
{
	MakeScript(SCRIPT_APP2);
	unsigned int nValue = 0x12345678;
	for (unsigned int nLba = 0; nLba <= 5; nLba++)
	{
		EXPECT_CALL(mockSSD, Read).WillRepeatedly(Return(nValue));
	}
	EXPECT_THAT(pTestScript->DoScript(), Eq(true));
}

TEST_F(TestScriptAppFixture, TestScriptApp2_CheckWhenVerifyFail)
{
	MakeScript(SCRIPT_APP2);
	unsigned int nValue = 0xAAAABBBB;
	for (unsigned int nLba = 0; nLba <= 5; nLba++)
	{
		EXPECT_CALL(mockSSD, Read(nLba)).WillRepeatedly(Return(nValue));
	}
	EXPECT_THAT(pTestScript->DoScript(), Eq(false));
}

class SSDFixture : public testing::Test
{
public:
	void SetUp() override
	{
		fResultFile.open(sResultFileName);
		remove(sNANDFileName.c_str());
	}

	void TearDown() override
	{
		if (fResultFile.is_open())
		{
			fResultFile.close();
		}
	}

	SSD ssd;
	const unsigned int INVALID_DATA = 0x00000000;
	const unsigned int INVALID_LBA = 0xFF;
	string sResultFileName = "result.txt";
	string sNANDFileName = "nand.txt";
	ifstream fResultFile;
};

TEST_F(SSDFixture, Read_LBANeverBeenWritten)
{
	EXPECT_EQ(INVALID_DATA, ssd.Read(0));
}

TEST_F(SSDFixture, Read_CreateResultFile)
{
	ssd.Read(0);

	if (fResultFile)
	{
		unsigned int nValue = -1;
		fResultFile >> hex >> nValue;

		EXPECT_EQ(INVALID_DATA, nValue);
	}
	else
	{
		FAIL() << sResultFileName << " not exist.";
	}
}

TEST_F(SSDFixture, Read_ReadAfterWrite)
{
	unsigned int nAddr = 0;
	unsigned int nData = 0x1122AABB;

	ssd.Write(nAddr, nData);
	EXPECT_EQ(nData, ssd.Read(nAddr));
}

TEST_F(SSDFixture, Read_InvalidLBA)
{
	EXPECT_THROW(ssd.Read(INVALID_LBA), exception);
}

TEST_F(SSDFixture, Write_InvalidLBA)
{
	unsigned int nData = 0x1122AABB;
	EXPECT_THROW(ssd.Write(INVALID_LBA, nData), exception);
}

TEST_F(SSDFixture, WriteSDDNormalTest) {
	bool isFirstWrite = true;
	SSD ssd;
	ifstream fin;
	ofstream fout;
	string index, value;
	unordered_map<unsigned int, unsigned int> umExpectedDataSet;
	unordered_map<unsigned int, unsigned int> umActualDataSet;

	umExpectedDataSet.insert({ 0, 0x1122AABB });
	umExpectedDataSet.insert({ 3, 0x11CCAABB });

	fout.open("nand.txt");

	for (const auto& pair : umExpectedDataSet) {
		fout << pair.first << " " << "0x" << hex << pair.second << endl;
	}

	fout.close();

	umExpectedDataSet.insert({ 17, 0x11AAFF44 });
	ssd.Write(17, 0x11AAFF44);

	fin.open("nand.txt");
	while (!fin.eof())
	{
		fin >> index >> value;
		umActualDataSet.insert({ stoi(index), stoi(value, nullptr, 16) });
	}
	fin.close();

	if (umExpectedDataSet.size() != umActualDataSet.size()) {
		cout << "Err: Size is different" << endl;
		FAIL();
	}

	for (const auto& pair : umExpectedDataSet) {
		auto it = umActualDataSet.find(pair.first);
		if (it == umActualDataSet.end() || it->second != pair.second) {
			cout << "Err: ActualDataSet has wrong value" << endl;
			FAIL();
		}
	}
}